#ifndef MESSAGEHOLDER_H
#define MESSAGEHOLDER_H

#include <QQueue>
#include <QTime>

struct MessageInfo {
  long id = -1;
  QByteArray text;
  QByteArray nick;
  long priority = -1;
  long senderNum = -1;
  QTime receiveTime;

  MessageInfo() {};
  MessageInfo(long nId, QByteArray nText, QByteArray nNick, long nPriority, long nSenderNum, QTime nReceiveTime)
    : id(nId)
    , text(nText)
    , nick(nNick)
    , priority(nPriority)
    , senderNum(nSenderNum)
    , receiveTime(nReceiveTime)
  {};
};

typedef QQueue<MessageInfo> MessageQueue;

class MessageHolder
{
public:
  MessageHolder();
  ~MessageHolder();

  void add(const MessageInfo& message);
  void add(long id, const QByteArray& text, const QTime& receiveTime
           , long priority = 0, const QByteArray& nick = "", long senderNum = 0);
  void dequeue();

  bool isEmpty() const;
  const MessageInfo& first() const;

private:
  MessageQueue messageQueue_;
};

#endif // MESSAGEHOLDER_H
