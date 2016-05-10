#include "MessageHolder.h"

MessageHolder::MessageHolder()
{

}

MessageHolder::~MessageHolder()
{

}

void MessageHolder::add(const MessageInfo &message)
{
  messageQueue_.enqueue(message);
}

void MessageHolder::add(long id, const QByteArray &text, const QTime &receiveTime, long priority
                        , const QByteArray &nick, long senderNum)
{
  add(MessageInfo(id, text, nick, priority, senderNum, receiveTime));
}

void MessageHolder::dequeue()
{
  if (!messageQueue_.empty())
  {
    messageQueue_.dequeue();
  }
  else
  {
    qWarning() << "ERROR dequeue message. Something went wrong";
  }
}

bool MessageHolder::isEmpty() const
{
  return messageQueue_.isEmpty();
}

const MessageInfo& MessageHolder::first() const
{
  return messageQueue_.first();
}
