#include "Boss.h"

#include <QTime>
#include <QSettings>
#include <QGuiApplication>

#include <SMSObjectManager.h>
#include <MessageHolder.h>
#include <NetworkManager.h>

#include <LogHelper.h>

Boss::Boss(QObject *parent)
  : QObject(parent)
  , smsObjectManager_(nullptr)
  , isConfirmed_(false)
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

  QObject::connect(smsObjectManager_, SIGNAL(titleCheck(bool)), this, SLOT(onTitleCheck(bool)));
  QObject::connect(smsObjectManager_, SIGNAL(messageSet(long)), this, SLOT(onMessageSet(long)));
  QObject::connect(smsObjectManager_, SIGNAL(messageDone(long)), this, SLOT(onMessageDone(long)));
  QObject::connect(smsObjectManager_, SIGNAL(messageFailed(long)), this, SLOT(onMessageFailed(long)));
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

      networkManager_->emptyXmlRequest([this](const OpResult& opResult)
      {
        switch (opResult)
        {
          case OpResult::EmptyData:
          case OpResult::Success:
          {
            onEmptyXml();
          }
          break;

          case OpResult::RequestError:
          {
            onServerError();
          }
          break;

          case OpResult::ParseError:
          break;
        }
      });
    }
  }
  else
  {
    Loggers::app->debug() << "title is dead";
    isConfirmed_ = false;
    //TODO is server need to know this?
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

void Boss::onEmptyXml()
{
  serverActive(true);
  if (!unshownMessages_->isEmpty())
  {
    Loggers::app->warn() << "some messages was not shown, show them first";
    showNextMessage();
  }
  else
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
          onParseError();
        }
        break;
      }
    });
  }
}

void Boss::onEmptyMessageXml()
{
  serverActive(true);

  networkManager_->serviceMessageRequest([this](const OpResult& opResult, const Message& message)
  {
    switch (opResult)
    {
      case OpResult::EmptyData:
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

      case OpResult::ParseError:
      {
        onParseError();
      }
      break;
    }
  });
}

void Boss::onUserMessageReceived(const Message& message)
{
  serverActive(true);
  //addSplittedMessage(id, message, time, priority);
  unshownMessages_->add(message);
  showNextMessage();
}

void Boss::onServiceMessageReceived(const Message& message)
{
  serverActive(true);
  //addSplittedMessage(id, message, time);
  unshownMessages_->add(message);
  showNextMessage();
}

void Boss::onServerError()
{
  serverActive(false);
  //TODO ?
}

void Boss::onMobileError()
{
  //TODO ?
}

void Boss::onParseError()
{
  //TODO ?
}

void Boss::showNextMessage()
{
  if (unshownMessages_->isEmpty())
  {
    messageChanged(-1, "", -1);

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
          onParseError();
        }
        break;
      }
    });

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
