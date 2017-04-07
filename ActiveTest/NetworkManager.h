#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <functional>

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QMap>
#include <QMutex>

#include <Message.h>

enum class OpResult { Success, EmptyData, ParseError, RequestError };

template <typename ...Args>
using Callback = std::function<void(const OpResult& error, const Args& ... args)>;

class NetworkManager : public QObject
{
  Q_OBJECT

public:
  explicit NetworkManager(QObject *parent = 0);  

public:
  void emptyXmlRequest(const Callback<>& callback);
  void userMessageRequest(const Callback<const Message&>& callback);
  void messageSetConfirm(long id, const Callback<>& callback);
  void serviceMessageRequest(const Callback<const Message&>& callback);

  void sendMessageToMobile(const Message& message);
  void sendServiceMessageToMobile(const Message& message);

private:
  QNetworkReply* sendRequest(const char* type);

  template <typename ParseCallback>
  void parseMessageXml(const QByteArray& xmlString, const ParseCallback& callback);

  template <typename ParseCallback>
	void parseServiceMessageXml(const QByteArray& xmlString, const ParseCallback& callback);

	template <typename Handler>
	void restartTimer(QNetworkReply* reply, const Handler& handler);
	void abortTimer(QNetworkReply* reply);

	void addConnection(QNetworkReply* reply, const QMetaObject::Connection& connection);
	void addConnection(QNetworkReply* reply, const QVector<QMetaObject::Connection>& connections);
	void clearConnections(QNetworkReply* reply);

private:
  QNetworkAccessManager* manager_;
	QTimer* timer_;
	QMap<QNetworkReply*, QVector<QMetaObject::Connection>> replyConnections_;
	QMutex replyMutex_;

  struct RequestData
  {
    QString server;
    QString password;
    int id;
    QByteArray hash;
    bool logging = false;
  } cache_;

  struct MobileParams
  {
    QString server;
    bool use;
  } mobileParams_;
};

#endif // NETWORKMANAGER_H
