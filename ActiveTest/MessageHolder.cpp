#include "MessageHolder.h"

#include <LogHelper.h>

MessageHolder::MessageHolder()
{

}

MessageHolder::~MessageHolder()
{

}

void MessageHolder::add(const Message &message)
{
  messageQueue_.enqueue(message);
}

void MessageHolder::add(long id, const QByteArray &text, const QTime &receiveTime, long priority
                        , const QByteArray &nick, long senderNum)
{
  add(Message(id, text, nick, priority, senderNum, receiveTime));
}

void MessageHolder::dequeue()
{
  if (!messageQueue_.empty())
  {
    messageQueue_.dequeue();
  }
  else
  {
    Loggers::app->warn() << "ERROR dequeue message. Something went wrong";
  }
}

bool MessageHolder::isEmpty() const
{
  return messageQueue_.isEmpty();
}

const Message& MessageHolder::first() const
{
  return messageQueue_.first();
}
