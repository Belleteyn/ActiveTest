#ifndef SMSOBJECTMANAGER_H
#define SMSOBJECTMANAGER_H

#include "SMSObjMgr.tlh"
#include <QObject>
#include <QTimer>

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
  void messageSet();
  void messageDone();

private slots:
  void onTitleMonitorTimer();

private:
  bool initSmsObjectManager();
  void showSmsObjects(); //just info

  void updateSMSObject(const IUnknownPtr& iUnknown);
  void resetSMSObject();
  void resetCallback();

private:
  ISMSObjectsManagerPtr iSMSObjectManager_;
  class CSMSCallBack* iSMSCallBack_;

  ISMSObjectPtr iSMSObject_;
  QTimer* titleMonitorTimer_;
};

#endif // SMSOBJECTMANAGER_H
