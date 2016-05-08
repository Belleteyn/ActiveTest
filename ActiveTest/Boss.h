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

private slots:
  //from sms
  void onTitleCheck(bool isTitleAlive);
  void onMessageSet();
  void onMessageDone();
  void onMessageFailed();

  //from server
  void onEmptyXml();
  void onEmptyMessageXml();
  void onUserMessageReceived(long id, const QByteArray& message, const QTime& time, int priority);
  void onServiceMessageReceived(long id, const QByteArray& message, const QTime& time);

private:
  void showNextMessage() const;

private:
  class SMSObjectManager* smsObjectManager_;
  bool isConfirmed_;

  class MessageHolder* unshownMessages_;

  class ServerTest* serverTest_;
};

#endif // BOSS_H
