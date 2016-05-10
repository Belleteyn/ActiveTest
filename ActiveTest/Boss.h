#ifndef BOSS_H
#define BOSS_H

#include <QObject>

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
  void onMessageSet();
  void onMessageDone();
  void onMessageFailed();

  //from server
  void onEmptyXml();
  void onEmptyMessageXml();
  void onUserMessageReceived(long id, const QByteArray& message, const QTime& time, long priority);
  void onServiceMessageReceived(long id, const QByteArray& message, const QTime& time);

private:
  void showNextMessage();
  void addSplittedMessage(long id, const QByteArray &message, const QTime &time, long priority = 0);
  QByteArray formMessage(QList<QByteArray>* splittedMessage, long id, long priority);

private:
  class SMSObjectManager* smsObjectManager_;
  bool isConfirmed_;

  class MessageHolder* unshownMessages_;

  class ServerTest* serverTest_;
};

#endif // BOSS_H
