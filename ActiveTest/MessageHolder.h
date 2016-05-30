#ifndef MESSAGEHOLDER_H
#define MESSAGEHOLDER_H

#include <Message.h>

typedef QQueue<Message> MessageQueue;

class MessageHolder
{
public:
  MessageHolder();
  ~MessageHolder();

  void add(const Message& message);
  void add(long id, const QByteArray& text, const QTime& receiveTime
           , long priority = 0, const QByteArray& nick = "", long senderNum = 0);
  void dequeue();

  bool isEmpty() const;
  const Message& first() const;

private:
  MessageQueue messageQueue_;
};

#endif // MESSAGEHOLDER_H
