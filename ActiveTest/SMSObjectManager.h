#ifndef SMSOBJECTMANAGER_H
#define SMSOBJECTMANAGER_H

#include "SMSObjMgr.tlh"
#include <QVector>
#include <QObject>
#include <QJSValue>
#include <QTimer>

//#include <CSMSCallback.h>
#include "smscallback.h"

class SMSObjectManager : public QObject
{
  Q_OBJECT

public:
  explicit SMSObjectManager(QObject *parent = 0);
  ~SMSObjectManager();

  /* initialize */
  bool init();
  void start();
  //

  void setMessage(int id, const QString message, int priority, int sender
                  , const QTime& receiveTime);

signals:
  void titleCheck(bool isTitleAlive);

private slots:
  void onTitleMonitorTimer();

private:
  bool initSmsObjectManager();
  bool initSmsCallback();
  void showSmsObjects(); //just info

  void resetSMSObject();

private:
  ISMSObjectsManagerPtr iSMSObjectManager_;
  ISMSCallBackPtr iSMSCallBack_;

  ISMSObjectPtr iSMSObject_;
  QTimer* titleMonitorTimer_;
};

#endif // SMSOBJECTMANAGER_H
