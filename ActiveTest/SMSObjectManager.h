#ifndef SMSOBJECTMANAGER_H
#define SMSOBJECTMANAGER_H

#include "SMSObjMgr.tlh"
#include <QObject>
#include <QTimer>

class CSMSCallBack;

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

  bool testMessage(long id, const QByteArray& message, long priority);
  void setMessage(long id, const QByteArray& message, long priority);

signals:
  void titleCheck(bool isTitleAlive);
  void messageSet(long id);
  void messageFailed(long id);
  void messageDone(long id);

private slots:
  void onTitleMonitorTimer();

private:
  bool initSmsObjectManager();
  void showSmsObjects(); //just info

  void updateSMSObject(ISMSObjectPtr& smsObj, const IUnknownPtr& iUnknown, CSMSCallBack *callback);
  void resetSMSObject();
  void resetCallback();

  bool testMessage(const ISMSObjectPtr& smsObj, long id, const QByteArray& message, long priority);
  void setMessage(const ISMSObjectPtr& smsObj, long id, const QByteArray& message, long priority);

private:
  ISMSObjectsManagerPtr iSMSObjectManager_;
  CSMSCallBack* iSMSCallBack_;
  CSMSCallBack* iSMSVipCallBack_;

  ISMSObjectPtr iSMSObject_;
  ISMSObjectPtr iSMSVipObject_;

  QTimer* titleMonitorTimer_;
  long vipPriority_;
};

#endif // SMSOBJECTMANAGER_H
