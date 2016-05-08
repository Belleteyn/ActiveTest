#include "servertest.h"

#include <random>

ServerTest::ServerTest(QObject *parent)
  : QObject(parent)
  , messageIdCounter_(0)
{

}

void ServerTest::userMessageRequest()
{
  messageIdCounter_++;
  int isHaveUserMessages = rand() % 2;
  if (isHaveUserMessages == 0)
  {
    userMessage(messageIdCounter_, "user message", QTime::currentTime());
  }
  else
  {
    qDebug() << "no user messages";
    emptyMessageXml();
  }
}

void ServerTest::serviceMessageRequest()
{
  messageIdCounter_++;
  serviceMessage(messageIdCounter_, "service message", QTime::currentTime());
}
