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

  if (randomBool()) //have user messages
  {
    userMessage(randomId(), "user message", QTime::currentTime(), randomPrior());
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
  serviceMessage(randomId(), "service message", QTime::currentTime());
}

bool ServerTest::randomBool()
{
  int var = rand() % 2;
  return (var == 0);
}

long ServerTest::randomId()
{
  if (randomBool()) // good id
  {
    if (sizeof(int) < sizeof(long))
      return (static_cast<long>(rand()) << (sizeof(int) * 8)) | rand();

    return rand();
  }
  else
  {
    return -rand();
    }
}

int ServerTest::randomPrior()
{
  if (randomBool()) // good prior
  {
    return rand() % 8;
  }
  else
  {
    int prior;
    if (randomBool()) // too big
    {
      prior = rand() + 7;
    }
    else //negative
    {
      prior = rand() % 8 - 8;
    }

    qDebug() << "bad priority" << prior;
    return prior;
  }
}
