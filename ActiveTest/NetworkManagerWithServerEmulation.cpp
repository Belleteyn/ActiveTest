#include "NetworkManagerWithServerEmulation.h"

#include <QFile>
#include <random>
#include <QSettings>

#include <QGuiApplication>

#include <LogHelper.h>

NetworkManagerWithServerEmulation::NetworkManagerWithServerEmulation(QObject *parent)
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

void NetworkManagerWithServerEmulation::emptyXmlRequest(const Callback<> &callback)
{
  static QSettings settings(QGuiApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
  serverAlive_ = settings.value("test/serverAlive").toBool();

  Loggers::net->info() << "PING";

  if (callback)
  {
    callback(serverAlive_ ? OpResult::Success : OpResult::RequestError);
  }
}

void NetworkManagerWithServerEmulation::userMessageRequest(const Callback<const Message &> &callback)
{
  if (!serverAlive_)
  {
    callback(OpResult::RequestError, Message());
    return;
  }

  messageIdCounter_++;

  if (randomBool()) //have user messages
  {
    Message message;

    message.id = randomId();
    message.text = randomMessage();
    message.receiveTime = QTime::currentTime();
    message.priority = randomPrior();

    callback(OpResult::Success, message);
  }
  else
  {
    if (callback)
    {
      callback(randomBool() ? OpResult::EmptyData : OpResult::RequestError, Message());
    }
  }
}

void NetworkManagerWithServerEmulation::messageSetConfirm(long id, const Callback<> &callback)
{

}

void NetworkManagerWithServerEmulation::serviceMessageRequest(const Callback<const Message &> &callback)
{
  if (!serverAlive_)
  {
    callback(OpResult::RequestError, Message());
    return;
  }

  messageIdCounter_++;

  if (randomBool()) //have user messages
  {
    Message message;

    message.id = randomId();
    message.text = "service message";
    message.priority = 0;

    callback(OpResult::Success, message);
  }
  else
  {
    if (callback)
    {
      callback(randomBool() ? OpResult::ParseError : OpResult::RequestError, Message());
    }
  }
}

void NetworkManagerWithServerEmulation::sendMessageToMobile(const Message &message)
{

}

void NetworkManagerWithServerEmulation::sendServiceMessageToMobile(const Message &message)
{

}

bool NetworkManagerWithServerEmulation::randomBool()
{
  int var = rand() % 2;
  return (var == 0);
}

long NetworkManagerWithServerEmulation::randomId()
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

int NetworkManagerWithServerEmulation::randomPrior()
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

QByteArray NetworkManagerWithServerEmulation::randomMessage()
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
