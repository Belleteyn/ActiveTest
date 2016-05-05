#include "Boss.h"

#include <QTime>

#include <SMSObjectManager.h>

Boss::Boss(QObject *parent)
  : QObject(parent)
  , smsObject_(new SMSObjectManager(parent))
{
  if (!smsObject_->init())
    return;

  QObject::connect(smsObject_, SIGNAL(titleCheck(bool)), this, SLOT(onTitleCheck(bool)));
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
    qDebug() << "title is alive";
    //TODO send confirmation to server 1 time

    onMessageReceived();
  }
  else
  {
    qDebug() << "title id dead";
    //TODO ???
  }
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
    smsObject_->setMessage(0, "message", 0, 0, QTime::currentTime());
  }
  else
  {
    qWarning() << "no sms object manager";
    //TODO error handle
  }
}
