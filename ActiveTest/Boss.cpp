#include "Boss.h"

#include <QTime>
#include <QSettings>
#include <QGuiApplication>

#include <SMSObjectManager.h>
#include <MessageHolder.h>
#include <NetworkManager.h>

#include <servertest.h>

Boss::Boss(QObject *parent)
  : QObject(parent)
  , smsObjectManager_(nullptr)
  , isConfirmed_(false)
  , unshownMessages_(nullptr)
  , serverTest_(nullptr)
  , networkManager_(nullptr)
{
}

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
    serverTest_ = new ServerTest(this);
  }
  catch (std::bad_alloc&)
  {
    qFatal("Fatal error: unable to create SMSObjectManager or MessageHolder;");
    return false;
  }

  if (!smsObjectManager_->init())
  {
    qFatal("Fatal error: unable to init SMSObjectManager;");
    return false;
  }

  QObject::connect(networkManager_, &NetworkManager::emptyXml, this, &Boss::onEmptyXml);
  QObject::connect(networkManager_, &NetworkManager::emptyMessageXml, this, &Boss::onEmptyMessageXml);
  QObject::connect(networkManager_, &NetworkManager::serviceMessage, this, &Boss::onServiceMessageReceived);
  QObject::connect(networkManager_, &NetworkManager::userMessage, this, &Boss::onUserMessageReceived);

  QObject::connect(networkManager_, &NetworkManager::pingError, this, &Boss::onServerError);
  QObject::connect(networkManager_, &NetworkManager::messageError, this, &Boss::onServerError);
  QObject::connect(networkManager_, &NetworkManager::serviceMessageError, this, &Boss::onServerError);
  QObject::connect(networkManager_, &NetworkManager::messageConfirmError, this, &Boss::onServerError);

  QObject::connect(networkManager_, &NetworkManager::mobileMessageError, this, &Boss::onMobileError);
  QObject::connect(networkManager_, &NetworkManager::parseMessageError, this, &Boss::onParseError);
  QObject::connect(networkManager_, &NetworkManager::parseServiceMessageError, this, &Boss::onParseError);


  QObject::connect(serverTest_, &ServerTest::emptyXml, this, &Boss::onEmptyXml);
  QObject::connect(serverTest_, &ServerTest::emptyMessageXml, this, &Boss::onEmptyMessageXml);
  QObject::connect(serverTest_, &ServerTest::serviceMessage, this, &Boss::onServiceMessageReceived);
  QObject::connect(serverTest_, &ServerTest::userMessage, this, &Boss::onUserMessageReceived);

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
      //networkManager_->emptyXmlRequest();
      serverTest_->emptyXmlRequest();
    }
  }
  else
  {
    qDebug() << "title is dead";
    isConfirmed_ = false;
    //TODO is server need to know this?
  }

  titleActive(isTitleAlive);
}

void Boss::onMessageSet(long id)
{
  if (!unshownMessages_->isEmpty())
  {
    MessageInfo message = unshownMessages_->first();
    if (message.id == id)
    {
      networkManager_->messageSetConfirm(id);
      messageChanged(message.id, message.text, message.priority);
    }
    else
    {
      qWarning() << "WTF? top message is not equal to set; ignore";
    }
  }
  else
  {
    qWarning() << "WTF? Message set, but queue is empty; ignore";
  }
}

void Boss::onMessageDone(long id)
{
  if (!unshownMessages_->isEmpty())
  {
    MessageInfo message = unshownMessages_->first();
    if (message.id == id)
    {
      unshownMessages_->dequeue();
      showNextMessage();
    }
    else
    {
      qWarning() << "top message is not equal to done. it can be when app started while message have been shown; ignore";
    }
  }
  else
  {
    qWarning() << "WTF? Message done, but queue is empty; ignore";
  }
}

void Boss::onMessageFailed(long id)
{
  qWarning() << "failed to set message" << id << ", will try again on message callback";
}

void Boss::onEmptyXml()
{
  if (!unshownMessages_->isEmpty())
  {
    qWarning() << "some messages was not shown, show them first";
    showNextMessage();
  }
  else
  {
    networkManager_->userMessageRequest();
    serverTest_->userMessageRequest();
  }
}

void Boss::onEmptyMessageXml()
{
  networkManager_->serviceMessageRequest();
  serverTest_->serviceMessageRequest();
}

void Boss::onUserMessageReceived(long id, const QByteArray &message, const QTime &time, long priority)
{
  //addSplittedMessage(id, message, time, priority);
  unshownMessages_->add(id, message, time, priority);
  showNextMessage();
}

void Boss::onServiceMessageReceived(long id, const QByteArray &message, const QTime &time)
{
  //addSplittedMessage(id, message, time);
  unshownMessages_->add(id, message, time, 0);
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
    networkManager_->userMessageRequest();
    serverTest_->userMessageRequest();
    return;
  }

  MessageInfo message = unshownMessages_->first();
  smsObjectManager_->setMessage(message.id, message.text, message.priority);

  QSettings settings(QGuiApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
  bool isMobileUsing = settings.value("Mobile/use").toBool();
  if (isMobileUsing)
  {
    qDebug() << "mobile app is using";
    //TODO send to mobile app
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
