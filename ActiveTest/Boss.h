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
  void serverActive(bool isServerActive);
  void messageChanged(long currentId, const QString& currentMessage, long currentPriority);

private slots:
  //from sms
  void onTitleCheck(bool isTitleAlive);
  void onMessageSet(long id);
  void onMessageDone(long id);
  void onMessageFailed(long id);

  //from server
  void onEmptyXml();
  void onEmptyMessageXml();
  void onUserMessageReceived(const Message& message);
  void onServiceMessageReceived(const Message& message);

  void onServerError();
  void onMobileError();
  void onParseError();

private:
  void showNextMessage();
  void addSplittedMessage(long id, const QByteArray &message, const QTime &time, long priority = 0);
  QByteArray formMessage(QList<QByteArray>* splittedMessage, long id, long priority);

private:
  class SMSObjectManager* smsObjectManager_;
  bool isConfirmed_;

  class MessageHolder* unshownMessages_;

  class ServerTest* serverTest_;
  class NetworkManager* networkManager_;
};

#endif // BOSS_H
