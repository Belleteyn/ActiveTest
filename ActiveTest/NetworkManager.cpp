#include "NetworkManager.h"

#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QCryptographicHash>
#include <QGuiApplication>
#include <QXmlStreamReader>
#include <QSettings>

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

void NetworkManager::ping()
{
  QNetworkReply* reply = sendRequest("PING");
  if (reply)
  {
    QObject::connect(reply, &QNetworkReply::finished, [=]()
    {
      if(reply->error() == QNetworkReply::NoError)
      {
        emptyXml();
      }
      else
      {
        qDebug() << "gor error in PING";
      }
      reply->deleteLater();
    });
  }
}

void NetworkManager::getMessages()
{
  QNetworkReply* reply = sendRequest("GET_MESSAGES");
  if (reply)
  {
    QObject::connect(reply, &QNetworkReply::finished, [=]()
    {
      if(reply->error() == QNetworkReply::NoError)
      {
        parseMessageXml(reply->readAll());
      }
      else
      {
        qDebug() << "gor error in GET_MESSAGES";
      }
      reply->deleteLater();
    });
  }
}

void NetworkManager::messageShown(long id)
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
        qDebug() << "gor error in MESSAGE_SHOWN";
      }
      reply->deleteLater();
    });
  }
}

void NetworkManager::getServiceMessage()
{
  QNetworkReply* reply = sendRequest("GET_SERVICE_MESSAGE");
  if (reply)
  {
    QObject::connect(reply, &QNetworkReply::finished, [=]()
    {
      if(reply->error() == QNetworkReply::NoError)
      {
        parseServiceMessageXml(reply->readAll());
      }
      else
      {
        qDebug() << "gor error in GET_SERVICE_MESSAGE";
      }
      reply->deleteLater();
    });
  }
}

void NetworkManager::sendMessageToMobile(const QByteArray& urlentext, long id, int priority, const QString& phone, const QTime& time)
{
  QNetworkRequest request;

  request.setUrl(QUrl(mobileParams_.server));
  request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));

  QUrlQuery params;
  params.addQueryItem("text", urlentext);
  params.addQueryItem("id", QString::number(id));
  params.addQueryItem("priority", QString::number(priority));
  params.addQueryItem("phone", phone);
  params.addQueryItem("time", time.toString());

  QNetworkReply* reply = manager_->post(request, params.query().toUtf8());
  if (reply)
  {
    QObject::connect(reply, &QNetworkReply::finished, [=]()
    {
      if(reply->error() != QNetworkReply::NoError)
      {
        qDebug() << "gor error in sendMessageToMobile";
      }
      reply->deleteLater();
    });
  }
}

void NetworkManager::sendMessageToMobile(const QString& text, long id, int priority, const QString& phone, const QTime& time)
{
  sendMessageToMobile(QUrl::toPercentEncoding(text), id, priority, phone, time);
}

void NetworkManager::sendServiceMessageToMobile(const QByteArray& urlentext, const QTime& time)
{
  QNetworkRequest request;

  request.setUrl(QUrl(mobileParams_.server));
  request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));

  QUrlQuery params;
  params.addQueryItem("text", urlentext);
  params.addQueryItem("id", QString::number(7777));
  params.addQueryItem("priority", QString::number(0));
  params.addQueryItem("time", time.toString());

  QNetworkReply* reply = manager_->post(request, params.query().toUtf8());
  if (reply)
  {
    QObject::connect(reply, &QNetworkReply::finished, [=]()
    {
      if(reply->error() != QNetworkReply::NoError)
      {
        qDebug() << "gor error in sendServiceMessageToMobile";
      }
      reply->deleteLater();
    });
  }
}

void NetworkManager::sendServiceMessageToMobile(const QString& text, const QTime& time)
{
  sendServiceMessageToMobile(QUrl::toPercentEncoding(text), time);
}

QNetworkReply* NetworkManager::sendRequest(const char* type)
{
  QNetworkRequest request;

  qDebug() << cache_.server;

  request.setUrl(QUrl(cache_.server));
  request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));

  QUrlQuery params;
  params.addQueryItem("id", QString::number(cache_.id));
  params.addQueryItem("sig", cache_.hash);
  params.addQueryItem("req", QString(type).toUtf8());

  return manager_->post(request, params.query().toUtf8());
}

void NetworkManager::parseMessageXml(const QByteArray& xmlString)
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

          for (const QXmlStreamAttribute& attribute : attributes)
          {
            qDebug() << attribute.name() << " : " << attribute.value();
          }

          userMessage(attributes.value("id").toLong()
            , QUrl::fromPercentEncoding(attributes.value("text").toUtf8()).toUtf8()
            , QTime::fromString(attributes.value("rcv_time").toString())
            , attributes.value("priority").toInt());

          return;
        }
      }
      else
      {
        qDebug() << "cant find ~message~ tag";

        emptyMessageXml();
      }
    }
    else
    {
      qDebug() << "cant find ~messages~ tag";
    }
  }
  else
  {
    qDebug() << "empty xml";
  }
}

void NetworkManager::parseServiceMessageXml(const QByteArray& xmlString)
{
  QXmlStreamReader xml(xmlString);

  if (xml.readNextStartElement())
  {
    if (xml.name() == "message")
    {
      QXmlStreamAttributes attributes = xml.attributes();

      for (const QXmlStreamAttribute& attribute : attributes)
      {
        qDebug() << attribute.name() << " : " << attribute.value();
      }

      serviceMessage(attributes.value("id").toLong()
        , QUrl::fromPercentEncoding(attributes.value("text").toUtf8()).toUtf8()
        , QTime());

      return;
    }
    else
    {
      qDebug() << "cant find ~message~ tag";
    }
  }
  else
  {
    qDebug() << "empty xml";
  }
}
