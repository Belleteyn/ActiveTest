#ifndef MESSAGE_H
#define MESSAGE_H

#include <QQueue>
#include <QTime>

class Message
{
public:
  Message() {}
  Message(long nId, QByteArray nText, QByteArray nNick, long nPriority, long nSenderNum, QTime nReceiveTime)
    : id(nId)
    , text(nText)
    , nick(nNick)
    , priority(nPriority)
    , senderNum(nSenderNum)
    , receiveTime(nReceiveTime)
  {}

public:
  long id = -1;
  QByteArray text;
  QByteArray nick;
  long priority = -1;
  long senderNum = 0;
  QTime receiveTime;
};

#endif // MESSAGE_H
