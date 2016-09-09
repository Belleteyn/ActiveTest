#include "Boss.h"

#include <QTime>
#include <QTimer>
#include <QSettings>
#include <QGuiApplication>

#include <SMSObjectManager.h>
#include <MessageHolder.h>
#include <NetworkManager.h>

#include <LogHelper.h>

Boss::Boss(QObject *parent)
  : QObject(parent)
  , serverPingSheduler_(nullptr)
  , smsObjectManager_(nullptr)
  , isConfirmed_(false)
  , isServerActive_(false)
  , unshownMessages_(nullptr)
  , networkManager_(nullptr)
{}

Boss::~Boss()
{
  //sms object removes by parent

  if (unshownMessages_)
  {
    delete unshownMessages_;
    unshownMessages_ = nullptr;
  }
}

bool Boss::init()
{
  try
  {
    serverPingSheduler_ = new QTimer(this);
    unshownMessages_ = new MessageHolder();
    smsObjectManager_ = new SMSObjectManager(this);
    networkManager_ = new NetworkManager(this);
  }
  catch (std::bad_alloc&)
  {
    Loggers::app->error() << "Fatal error: unable to create SMSObjectManager or MessageHolder;";
    return false;
  }

  if (!smsObjectManager_->init())
  {
    Loggers::app->error() << "Fatal error: unable to init SMSObjectManager;";
    return false;
  }

  serverPingSheduler_->setInterval(1000);

  QObject::connect(smsObjectManager_, SIGNAL(titleCheck(bool)), this, SLOT(onTitleCheck(bool)));
  QObject::connect(smsObjectManager_, SIGNAL(messageSet(long)), this, SLOT(onMessageSet(long)));
  QObject::connect(smsObjectManager_, SIGNAL(messageDone(long)), this, SLOT(onMessageDone(long)));
  QObject::connect(smsObjectManager_, SIGNAL(messageFailed(long)), this, SLOT(onMessageFailed(long)));

  QObject::connect(serverPingSheduler_, &QTimer::timeout, this, &Boss::pingServer);

  QObject::connect(this, SIGNAL(serverActiveChanged(bool)), this, SLOT(onServerActiveChanged(bool)));

  smsObjectManager_->start();

  return true;
}

void Boss::onTitleCheck(bool isTitleAlive)
{
  if (isTitleAlive)
  {
    if (!isConfirmed_)
    {
      isConfirmed_ = true;

      serverPingSheduler_->start();
    }
  }
  else
  {
    Loggers::app->debug() << "title is dead";
    isConfirmed_ = false;

    serverPingSheduler_->stop();
    setServerActive(false);
  }

  titleActive(isTitleAlive);
}

void Boss::onMessageSet(long id)
{
  if (!unshownMessages_->isEmpty())
  {
    Message message = unshownMessages_->first();
    if (message.id == id)
    {
      networkManager_->messageSetConfirm(id, [this](const OpResult& opResult)
      {
        switch (opResult)
        {
          case OpResult::EmptyData:
          case OpResult::Success:
          case OpResult::ParseError:
          break;

          case OpResult::RequestError:
          {
            onServerError();
          }
          break;
        }
      });

      messageChanged(message.id, message.text, message.priority);
    }
    else
    {
      Loggers::app->warn() << "WTF? top message is not equal to set; ignore";
    }
  }
  else
  {
    Loggers::app->warn() << "WTF? Message set, but queue is empty; ignore";
  }
}

void Boss::onMessageDone(long id)
{
  if (!unshownMessages_->isEmpty())
  {
    Message message = unshownMessages_->first();
    if (message.id == id)
    {
      unshownMessages_->dequeue();
      showNextMessage();
    }
    else
    {
      Loggers::app->warn() << "top message is not equal to done. it can be when app started while message have been shown; ignore";
    }
  }
  else
  {
    Loggers::app->warn() << "WTF? Message done, but queue is empty; ignore";
  }
}

void Boss::onMessageFailed(long id)
{
  Loggers::app->warn() << "failed to set message " << id << ", will try again on message callback";
}

void Boss::userMessageRequest()
{
  networkManager_->userMessageRequest([this](const OpResult& opResult, const Message& message)
  {
    switch (opResult)
    {
      case OpResult::EmptyData:
      {
        onEmptyMessageXml();
      }
      break;

      case OpResult::Success:
      {
        onUserMessageReceived(message);
      }
      break;

      case OpResult::RequestError:
      {
        onServerError();
      }
      break;

      case OpResult::ParseError:
      {
        userMessageRequest();/*onParseError();*/
      }
      break;
    }
  });
}

void Boss::serviceMessageRequest()
{
  networkManager_->serviceMessageRequest([this](const OpResult& opResult, const Message& message)
  {
    switch (opResult)
    {
      case OpResult::EmptyData:
      case OpResult::ParseError:
      {
        userMessageRequest();
      }
      break;

      case OpResult::Success:
      {
        onServiceMessageReceived(message);
      }
      break;

      case OpResult::RequestError:
      {
        onServerError();
      }
      break;
    }
  });
}

void Boss::onServerActiveChanged(bool isServerActive)
{
  if (isServerActive)
  {
    if (!unshownMessages_->isEmpty())
    {
      Loggers::app->warn() << "some messages was not shown, show them first";
      showNextMessage();
    }
    else
    {
      userMessageRequest();
    }
  }
}

void Boss::onEmptyXml()
{
  setServerActive(true);
}

void Boss::onEmptyMessageXml()
{
  serviceMessageRequest();
}

void Boss::onUserMessageReceived(const Message& message)
{
  //addSplittedMessage(id, message, time, priority);

  unshownMessages_->add(message);
  showNextMessage();
}

void Boss::onServiceMessageReceived(const Message& message)
{
  //addSplittedMessage(id, message, time);

  unshownMessages_->add(message);
  showNextMessage();
}

void Boss::onServerError()
{
  setServerActive(false);
}

void Boss::onMobileError()
{
  //TODO Добавить поддержку отправки сообщений на мобильный
}

void Boss::pingServer()
{
  networkManager_->emptyXmlRequest([this](const OpResult& opResult)
  {
    switch (opResult)
    {
      case OpResult::EmptyData:
      case OpResult::Success:
      case OpResult::ParseError:
      {
        onEmptyXml();
      }
      break;

      case OpResult::RequestError:
      {
        onServerError();
      }
      break;
    }
  });
}

void Boss::setServerActive(bool isServerActive)
{
  if (isServerActive_ != isServerActive)
  {
    isServerActive_ = isServerActive;
    serverActiveChanged(isServerActive_);
  }
}

void Boss::showNextMessage()
{
  if (unshownMessages_->isEmpty())
  {
    messageChanged(-1, "", -1);

    userMessageRequest();

    return;
  }

  Message message = unshownMessages_->first();
  smsObjectManager_->setMessage(message.id, message.text, message.priority);

  QSettings settings(QGuiApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
  bool isMobileUsing = settings.value("Mobile/use").toBool();
  if (isMobileUsing)
  {
    Loggers::app->debug() << "mobile app is using";

    networkManager_->sendMessageToMobile(message);
  }
}

void Boss::addSplittedMessage(long id, const QByteArray &message, const QTime &time, long priority /* = 0*/)
{
  QList<QByteArray> spaceSplitted = message.split(' ');
  QByteArray newMessage = formMessage(&spaceSplitted, id, priority);
  while (newMessage.size() != 0)
  {
    unshownMessages_->add(id, newMessage, time, priority);
    newMessage = formMessage(&spaceSplitted, id, priority);
  }
}

QByteArray Boss::formMessage(QList<QByteArray> *splittedMessage, long id, long priority)
{
  QByteArray newMessage, test;
  while (!splittedMessage->empty())
  {
    test.append(splittedMessage->first());
    if (smsObjectManager_->testMessage(id, test, priority))
    {
      newMessage.append(splittedMessage->first());
      newMessage.append(' ');
      test.append(' ');
      splittedMessage->pop_front();
    }
    else
    {
      if (newMessage.size() == 0)
      {
        //TODO split by letters
      }
      break;
    }
  }
  return newMessage;
}
