#include "Boss.h"

#include <QTime>

#include <SMSObjectManager.h>

Boss::Boss(QObject *parent)
  : QObject(parent)
  , smsObject_(new SMSObjectManager(parent))
  , isConfirmed_(false)
  , messageIdCounter_(0)
{
  if (!smsObject_->init())
    return;

  QObject::connect(smsObject_, SIGNAL(titleCheck(bool)), this, SLOT(onTitleCheck(bool)));
  QObject::connect(smsObject_, SIGNAL(messageSet()), this, SLOT(onMessageSet()));
  QObject::connect(smsObject_, SIGNAL(messageDone()), this, SLOT(onMessageDone()));
  QObject::connect(smsObject_, SIGNAL(messageFailed()), this, SLOT(onMessageFailed()));
  smsObject_->start();
}

Boss::~Boss()
{
  //sms object removes by parent
}

void Boss::onTitleCheck(bool isTitleAlive)
{
  if (isTitleAlive)
  {
    if (!isConfirmed_)
    {
      isConfirmed_ = true;

      if (!savedMessage_.isEmpty())
      {
        qWarning() << "last message was not shown, show it first";
        smsObject_->setMessage(messageIdCounter_, savedMessage_, 0, 0, QTime::currentTime());
        savedMessage_.clear();
      }

      //TODO send confirmation to server 1 time
      //server send empty xml

      onMessageReceived();
    }
  }
  else
  {
    qDebug() << "title is dead";
    isConfirmed_ = false;
    //TODO ???
  }
}

void Boss::onMessageSet()
{
  //TODO send message confirmation to server
  qDebug() << "=== on message set ===";
}

void Boss::onMessageDone()
{
  //TODO message request
  qDebug() << "=== on message done ===";
  savedMessage_.clear();

  onMessageReceived();
}

void Boss::onMessageFailed()
{
  qWarning() << "failed to set message, try again";
  //TODO try to set first from saved queue
}

void Boss::onEmptyXml()
{
  //TODO message request
}

void Boss::onEmptyMessageXml()
{
  //TODO service message request
}

void Boss::onMessageReceived()
{
  if (smsObject_)
  {
    messageIdCounter_++;
    savedMessage_ = QString::number(messageIdCounter_).toLocal8Bit();
    smsObject_->setMessage(messageIdCounter_, "Лекции по многопоточному программированию на С/С++ от Техносферы.", 0, 0, QTime::currentTime());
  }
  else
  {
    qWarning() << "no sms object manager";
    //TODO error handle
  }
}
