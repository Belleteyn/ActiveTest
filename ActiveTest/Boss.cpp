#include "Boss.h"

#include <QTime>

#include <SMSObjectManager.h>

Boss::Boss(QObject *parent)
  : QObject(parent)
  , smsObject_(new SMSObjectManager(parent))
  , isConfirmed_(false)
{
  if (!smsObject_->init())
    return;

  QObject::connect(smsObject_, SIGNAL(titleCheck(bool)), this, SLOT(onTitleCheck(bool)));
  QObject::connect(smsObject_, SIGNAL(messageSet()), this, SLOT(onMessageSet()));
  QObject::connect(smsObject_, SIGNAL(messageDone()), this, SLOT(onMessageDone()));
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
      //TODO send confirmation to server 1 time
      //server send empty xml
      isConfirmed_ = true;
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
    smsObject_->setMessage(0, "long long long long long loooooooooooooooooooong meeeeeeeeeeeeeeeeeeeeeeessage meeeeeeeeeeeeeeeeeeeeeeessage meeeeeeeeeeeeeeeeeeeeeeessage", 0, 0, QTime::currentTime());
  }
  else
  {
    qWarning() << "no sms object manager";
    //TODO error handle
  }
}
