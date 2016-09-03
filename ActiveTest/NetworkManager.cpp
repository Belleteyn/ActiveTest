#include "NetworkManager.h"

#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QCryptographicHash>
#include <QGuiApplication>
#include <QXmlStreamReader>
#include <QSettings>

#include <LogHelper.h>

NetworkManager::NetworkManager(QObject *parent)
  : QObject(parent)
{
  QSettings settings(QGuiApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);

  mobileParams_.server = settings.value("Mobile/server").toString();
  mobileParams_.use = settings.value("Mobile/use").toBool();

  cache_.server = settings.value("Server/server").toString();
  cache_.password = settings.value("Server/password").toString();
  cache_.id = settings.value("Server/id").toInt();

  QCryptographicHash hash(QCryptographicHash::Md5);
  hash.addData(QString::number(cache_.id).toUtf8());
  hash.addData(cache_.password.toUtf8());
  hash.addData(QString::number(cache_.id).toUtf8());

  cache_.hash = hash.result().toHex();

  manager_ = new QNetworkAccessManager(this);
}

void NetworkManager::emptyXmlRequest(const Callback<>& callback)
{
  QNetworkReply* reply = sendRequest("PING");
  if (reply)
  {
    QObject::connect(reply, &QNetworkReply::finished, [=]()
    {
      if(reply->error() == QNetworkReply::NoError)
      {
        if (callback)
        {
          callback(OpResult::Success);
        }
      }
      else
      {
        Loggers::app->critical() << "gor error in PING" << reply->error();

        if (callback)
        {
          callback(OpResult::RequestError);
        }
      }
      reply->deleteLater();
    });
  }
}

void NetworkManager::userMessageRequest(const Callback<const Message&>& callback)
{
  QNetworkReply* reply = sendRequest("GET_MESSAGES");
  if (reply)
  {
    QObject::connect(reply, &QNetworkReply::finished, [=]()
    {
      if(reply->error() == QNetworkReply::NoError)
      {
        parseMessageXml(reply->readAll(), callback);
      }
      else
      {
        Loggers::app->warn() << "gor error in GET_MESSAGES";
        if (callback)
        {
          callback(OpResult::RequestError, Message());
        }
      }
      reply->deleteLater();
    });
  }
}

void NetworkManager::messageSetConfirm(long id, const Callback<>& callback)
{
  QNetworkRequest request;

  request.setUrl(QUrl(cache_.server));
  request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));

  QUrlQuery params;
  params.addQueryItem("id", QString::number(cache_.id));
  params.addQueryItem("sig", cache_.hash);
  params.addQueryItem("req", "MESSAGE_SHOWN");
  params.addQueryItem("req_data", QString::number(id) + "#");

  QNetworkReply* reply = manager_->post(request, params.query().toUtf8());
  if (reply)
  {
    QObject::connect(reply, &QNetworkReply::finished, [=]()
    {
      if(reply->error() != QNetworkReply::NoError)
      {
        Loggers::app->warn() << "gor error in MESSAGE_SHOWN";

        if (callback)
        {
          callback(OpResult::RequestError);
        }
      }
      reply->deleteLater();
    });
  }
}

void NetworkManager::serviceMessageRequest(const Callback<const Message&>& callback)
{
  QNetworkReply* reply = sendRequest("GET_SERVICE_MESSAGE");
  if (reply)
  {
    QObject::connect(reply, &QNetworkReply::finished, [=]()
    {
      if(reply->error() == QNetworkReply::NoError)
      {
        parseServiceMessageXml(reply->readAll(), callback);
      }
      else
      {
        Loggers::app->warn() << "gor error in GET_SERVICE_MESSAGE";
        if (callback)
        {
          callback(OpResult::RequestError, Message());
        }
      }
      reply->deleteLater();
    });
  }
}

void NetworkManager::sendMessageToMobile(const Message& message)
{
  QNetworkRequest request;

  request.setUrl(QUrl(mobileParams_.server));
  request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));

  QUrlQuery params;
  params.addQueryItem("text", QUrl::toPercentEncoding(message.text));
  params.addQueryItem("id", QString::number(message.id));
  params.addQueryItem("priority", QString::number(message.priority));
  params.addQueryItem("phone", QString::number(message.senderNum));
  params.addQueryItem("time", message.receiveTime.toString());

  QNetworkReply* reply = manager_->post(request, params.query().toUtf8());
  if (reply)
  {
    QObject::connect(reply, &QNetworkReply::finished, [=]()
    {
      if(reply->error() != QNetworkReply::NoError)
      {
        Loggers::app->warn() << "gor error in sendMessageToMobile";
      }
      reply->deleteLater();
    });
  }
}

void NetworkManager::sendServiceMessageToMobile(const Message& message)
{
  QNetworkRequest request;

  request.setUrl(QUrl(mobileParams_.server));
  request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));

  QUrlQuery params;
  params.addQueryItem("text", QUrl::toPercentEncoding(message.text));
  params.addQueryItem("id", QString::number(7777));
  params.addQueryItem("priority", QString::number(0));
  params.addQueryItem("time", message.receiveTime.toString());

  QNetworkReply* reply = manager_->post(request, params.query().toUtf8());
  if (reply)
  {
    QObject::connect(reply, &QNetworkReply::finished, [=]()
    {
      if(reply->error() != QNetworkReply::NoError)
      {
        Loggers::app->warn() << "gor error in sendServiceMessageToMobile";
      }
      reply->deleteLater();
    });
  }
}

QNetworkReply* NetworkManager::sendRequest(const char* type)
{
  QNetworkRequest request;

  request.setUrl(QUrl(cache_.server));
  request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));

  QUrlQuery params;
  params.addQueryItem("id", QString::number(cache_.id));
  params.addQueryItem("sig", cache_.hash);
  params.addQueryItem("req", QString(type).toUtf8());

  return manager_->post(request, params.query().toUtf8());
}

template <typename ParseCallback>
void NetworkManager::parseMessageXml(const QByteArray& xmlString, const ParseCallback& callback)
{
  QXmlStreamReader xml(xmlString);

  if (xml.readNextStartElement())
  {
    if (xml.name() == "messages")
    {
      if (xml.readNextStartElement())
      {
        if (xml.name() == "message")
        {
          QXmlStreamAttributes attributes = xml.attributes();

          if (callback)
          {
            Message message;

            message.id = attributes.value("id").toLong();
            message.text = QUrl::fromPercentEncoding(attributes.value("text").toUtf8()).toUtf8();
            message.receiveTime = QTime::fromString(attributes.value("rcv_time").toString());
            message.priority = attributes.value("priority").toInt();

            callback(OpResult::Success, message);
          }

          return;
        }
      }
      else
      {
        Loggers::app->debug() << "cant find ~message~ tag";

        if (callback)
        {
          callback(OpResult::EmptyData, Message());
        }
      }
    }
    else
    {
      Loggers::app->warn() << "cant find ~messages~ tag";
      if (callback)
      {
        callback(OpResult::ParseError, Message());
      }
    }
  }
  else
  {
    Loggers::app->warn() << "empty xml";
    if (callback)
    {
      callback(OpResult::ParseError, Message());
    }
  }
}

template <typename ParseCallback>
void NetworkManager::parseServiceMessageXml(const QByteArray& xmlString, const ParseCallback& callback)
{
  QXmlStreamReader xml(xmlString);

  if (xml.readNextStartElement())
  {
    if (xml.name() == "message")
    {
      QXmlStreamAttributes attributes = xml.attributes();

      if (callback)
      {
        Message message;

        message.id = attributes.value("id").toLong();
        message.text = QUrl::fromPercentEncoding(attributes.value("text").toUtf8()).toUtf8();
        message.priority = 0;

        callback(OpResult::Success, message);
      }

      return;
    }
    else
    {
      Loggers::app->warn() << "cant find ~message~ tag";
      if (callback)
      {
        callback(OpResult::ParseError, Message());
      }
    }
  }
  else
  {
    Loggers::app->warn() << "empty xml";
    if (callback)
    {
      callback(OpResult::ParseError, Message());
    }
  }
}
