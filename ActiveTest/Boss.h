#ifndef BOSS_H
#define BOSS_H

#include <QObject>

class Boss : public QObject
{
  Q_OBJECT
public:
  explicit Boss(QObject *parent = 0);
  ~Boss();

signals:

private slots:
  //from sms
  void onTitleCheck(bool isTitleAlive);

  //from server
  void onEmptyXml();
  void onEmptyMessageXml();
  void onMessageReceived();

private:
  class SMSObjectManager* smsObject_;
};

#endif // BOSS_H
