#include "servertest.h"

#include <QFile>
#include <random>

#include <QGuiApplication>

ServerTest::ServerTest(QObject *parent)
  : QObject(parent)
  , messageIdCounter_(0)
{
  qint64 messageNum = 1;
  messageMap_.insert(0, "Save the world!");

  QFile file(QGuiApplication::applicationDirPath() + "/the war of the worlds.xml");
  if (file.open(QIODevice::ReadOnly))
  {
    QByteArray data = file.readLine();
    while (data.size() != 0)
    {
      if(data.left(3) == "<p>")
      {
        data = data.mid(3, data.length() - 3 - 4);

        messageNum++;
        messageMap_.insert(messageNum, data);
      }
      data = file.readLine();
    }
    file.close();
  }
  else
  {
    qDebug() << "cant open file";
  }
}

void ServerTest::userMessageRequest()
{
  messageIdCounter_++;

  if (randomBool()) //have user messages
  {
    QByteArray message = randomMessage();
    userMessage(randomId(), message, QTime::currentTime(), randomPrior());
  }
  else
  {
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

    return prior;
  }
}

QByteArray ServerTest::randomMessage()
{
  qint64 id = rand() % messageMap_.size();
  if (id < messageMap_.size() && id >= 0 && !messageMap_.empty())
  {
    return messageMap_[id];
  }
  else
  {
    qWarning() << "empty message map";
    return messageMap_[0];
  }
}
