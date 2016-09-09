#ifndef BOSS_H
#define BOSS_H

#include <QObject>

#include <Message.h>

class Boss : public QObject
{
  Q_OBJECT

public:
  explicit Boss(QObject *parent = 0);
  ~Boss();

  bool init();

signals:
  void titleActive(bool isTitleActive);
  void serverActiveChanged(bool isServerActive);
  void messageChanged(long currentId, const QString& currentMessage, long currentPriority);
  void appendLogString(const QString& tag, const QString& message);

private slots:
  //from sms
  void onTitleCheck(bool isTitleAlive);
  void onMessageSet(long id);
  void onMessageDone(long id);
  void onMessageFailed(long id);
  void onServerActiveChanged(bool isServerActive);

  //from server
  void onEmptyXml();
  void onEmptyMessageXml();
  void onUserMessageReceived(const Message& message);
  void onServiceMessageReceived(const Message& message);

  void onServerError();
  void onMobileError();

private:
  void pingServer();
  void userMessageRequest();
  void serviceMessageRequest();

  void setServerActive(bool isServerActive);
  void showNextMessage();

  void addSplittedMessage(long id, const QByteArray &message, const QTime &time, long priority = 0);
  QByteArray formMessage(QList<QByteArray>* splittedMessage, long id, long priority);

private:
  class QTimer* serverPingSheduler_;
  class SMSObjectManager* smsObjectManager_;
  bool isConfirmed_;
  bool isServerActive_;

  class MessageHolder* unshownMessages_;
  class NetworkManager* networkManager_;
};

#endif // BOSS_H
