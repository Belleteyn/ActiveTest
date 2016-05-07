#include "Boss.h"

#include <QTime>
#include <QSettings>
#include <QGuiApplication>

#include <SMSObjectManager.h>
#include <MessageHolder.h>

Boss::Boss(QObject *parent)
  : QObject(parent)
  , smsObjectManager_(nullptr)
  , isConfirmed_(false)
  , unshownMessages_(nullptr)
  , messageIdCounter_(0)
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
  }
  catch (std::bad_alloc&)
  {
    qWarning() << "Fatal error: unable to create SMSObjectManager or MessageHolder;";
    return false;
  }

  if (!smsObjectManager_->init())
  {
    qWarning() << "Fatal error: unable to init SMSObjectManager;";
    return false;
  }

  QObject::connect(smsObjectManager_, SIGNAL(titleCheck(bool)), this, SLOT(onTitleCheck(bool)));
  QObject::connect(smsObjectManager_, SIGNAL(messageSet()), this, SLOT(onMessageSet()));
  QObject::connect(smsObjectManager_, SIGNAL(messageDone()), this, SLOT(onMessageDone()));
  QObject::connect(smsObjectManager_, SIGNAL(messageFailed()), this, SLOT(onMessageFailed()));
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

      //TODO send confirmation to server 1 time
      //server send empty xml
      onEmptyXml();
    }
  }
  else
  {
    qDebug() << "title is dead";
    isConfirmed_ = false;
  }
}

void Boss::onMessageSet()
{
  //TODO send message confirmation to server
}

void Boss::onMessageDone()
{
  unshownMessages_->dequeue();
  if (!unshownMessages_->isEmpty())
  {
    showNextMessage();
  }
  else
  {
    //TODO message request
    onMessageReceived();
  }
}

void Boss::onMessageFailed()
{
  qWarning() << "failed to set message, try again";
  showNextMessage();
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
    //TODO message request
    onMessageReceived();
  }
}

void Boss::onEmptyMessageXml()
{
  //TODO service message request
}

void Boss::onMessageReceived()
{
  messageIdCounter_++;
  unshownMessages_->add(messageIdCounter_, "Лекции по многопоточному программированию на С/С++ от Техносферы.", QTime::currentTime());
  showNextMessage();
}

void Boss::showNextMessage() const
{
  MessageInfo message = unshownMessages_->first();
  if (message.id != -1)
  {
    smsObjectManager_->setMessage(message.id, message.text, message.priority, message.senderNum, message.receiveTime);

    QSettings settings(QGuiApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    bool isMobileUsing = settings.value("Mobile/use").toBool();
    if (isMobileUsing)
    {
      qDebug() << "mobile app is using";
      //TODO send to mobile app
    }
  }
  else
  {
    qWarning() << "invalid message id (message queue is empty)";
    //this means that message queue is empty
    //TODO request message
  }
}
