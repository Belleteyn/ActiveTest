#include "NetworkManager.h"

#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QCryptographicHash>
#include <QGuiApplication>
#include <QXmlStreamReader>
#include <QSettings>
#include <QMutexLocker>

#include <LogHelper.h>

NetworkManager::NetworkManager(QObject *parent)
  : QObject(parent)
{
  QSettings settings(QGuiApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);

  mobileParams_.server = settings.value("Mobile/server").toString();
  mobileParams_.use = settings.value("Mobile/use").toBool();

  cache_.logging = settings.value("Other/networkRequestLogging").toBool();
  cache_.server = settings.value("Server/server").toString();
  cache_.password = settings.value("Server/password").toString();
  cache_.id = settings.value("Server/id").toInt();

  QCryptographicHash hash(QCryptographicHash::Md5);
  hash.addData(QString::number(cache_.id).toUtf8());
  hash.addData(cache_.password.toUtf8());
  hash.addData(QString::number(cache_.id).toUtf8());

  cache_.hash = hash.result().toHex();

  manager_ = new QNetworkAccessManager(this);
	timer_ = new QTimer(this);

	timer_->setSingleShot(true);
	timer_->setInterval(settings.value("Server/timeout", 5000).toInt());
}

void NetworkManager::emptyXmlRequest(const Callback<>& callback)
{
  QNetworkReply* reply = sendRequest("PING");
  if (reply)
	{
		auto handler = [=](QNetworkReply* reply)
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
				Loggers::app->critical() << "got error in PING " << reply->error();

				if (callback)
				{
					callback(OpResult::RequestError);
				}
			}
		};

		restartTimer(reply, handler);
  }
}

void NetworkManager::userMessageRequest(const Callback<const Message&>& callback)
{
  QNetworkReply* reply = sendRequest("GET_MESSAGES");
  if (reply)
	{
		auto handler = [=](QNetworkReply* reply)
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
		};

		restartTimer(reply, handler);
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

  if (cache_.logging)
  {
    Loggers::net->info() << params.query();
  }

  QNetworkReply* reply = manager_->post(request, params.query().toUtf8());
  if (reply)
	{
		auto handler = [=](QNetworkReply* reply)
		{
			if(reply->error() != QNetworkReply::NoError)
			{
				Loggers::app->warn() << "gor error in MESSAGE_SHOWN";

				if (callback)
				{
					callback(OpResult::RequestError);
				}
			}
		};

		restartTimer(reply, handler);
  }
}

void NetworkManager::serviceMessageRequest(const Callback<const Message&>& callback)
{
  QNetworkReply* reply = sendRequest("GET_SERVICE_MESSAGE");
  if (reply)
	{
		auto handler = [=](QNetworkReply* reply)
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
		};

		restartTimer(reply, handler);
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

  if (cache_.logging)
  {
    Loggers::net->info() << params.query();
  }

  QNetworkReply* reply = manager_->post(request, params.query().toUtf8());
  if (reply)
	{
		auto handler = [=](QNetworkReply* reply)
		{
			if(reply->error() != QNetworkReply::NoError)
			{
				Loggers::app->warn() << "gor error in sendMessageToMobile";
			}
		};

		restartTimer(reply, handler);
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

  if (cache_.logging)
  {
    Loggers::net->info() << params.query();
  }

  QNetworkReply* reply = manager_->post(request, params.query().toUtf8());
  if (reply)
	{
		auto handler = [=](QNetworkReply* reply)
		{
			if(reply->error() != QNetworkReply::NoError)
			{
				Loggers::app->warn() << "gor error in sendServiceMessageToMobile";
			}
		};

		restartTimer(reply, handler);
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

  if (cache_.logging)
  {
    Loggers::net->info() << params.query();
  }

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

            QString string = attributes.value("text").toString();
            string = string.replace(QChar('+'), "%20");

            message.id = attributes.value("id").toLong();
            message.text = QUrl::fromPercentEncoding(string.toUtf8()).toUtf8();
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

        QString string = attributes.value("text").toString();
        string = string.replace(QChar('+'), "%20");

        message.id = attributes.value("id").toLong();
        message.text = QUrl::fromPercentEncoding(string.toUtf8()).toUtf8();
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

template<typename Handler>
void NetworkManager::restartTimer(QNetworkReply* reply, const Handler& handler)
{
	auto onReplyFinished = [=]()
	{
		abortTimer(reply);

		handler(reply);

		reply->deleteLater();
	};

	auto onTimeOut = [reply]()
	{
		Loggers::net->trace() << "request time out. aborting";
		reply->abort();
	};

	QVector<QMetaObject::Connection> connections;

	connections << QObject::connect(timer_, &QTimer::timeout, onTimeOut);

	connections << QObject::connect(reply, &QNetworkReply::finished, onReplyFinished);

	connections << QObject::connect(reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error)
		, onReplyFinished);

	addConnection(reply, connections);

	timer_->start();
}

void NetworkManager::abortTimer(QNetworkReply* reply)
{
	if (timer_->isActive())
	{
		timer_->stop();

		Loggers::net->trace() << "timer stopped!";
	}

	clearConnections(reply);
}

void NetworkManager::addConnection(QNetworkReply* reply, const QMetaObject::Connection& connection)
{
	QMutexLocker locker(&replyMutex_);

	if (!replyConnections_.contains(reply))
	{
		replyConnections_.insert(reply, QVector<QMetaObject::Connection>());
	}

	replyConnections_[reply].append(connection);
	Loggers::net->trace() << "connection added";
}

void NetworkManager::addConnection(QNetworkReply *reply, const QVector<QMetaObject::Connection>& connections)
{
	QMutexLocker locker(&replyMutex_);

	replyConnections_.insert(reply, connections);

	Loggers::net->trace() << replyConnections_[reply].size() << " connections added";
}

void NetworkManager::clearConnections(QNetworkReply* reply)
{
	QMutexLocker locker(&replyMutex_);

	if (replyConnections_.contains(reply))
	{
		auto& connections = replyConnections_[reply];
		for (const auto& connection : connections)
		{
			if (QObject::disconnect(connection))
			{
				Loggers::net->trace() << "connection removed";
			}
		}

		replyConnections_.remove(reply);
	}
	else
	{
		Loggers::net->trace() << "no connections for this reply";
	}
}
