#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <functional>

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

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

signals:
  void emptyXml();
  void emptyMessageXml();
  void serviceMessage(long id, const QByteArray& message, const QTime& time);
  void userMessage(long id, const QByteArray& message, const QTime& time, int priority);

  void pingError();
  void messageError();
  void serviceMessageError();
  void messageConfirmError();
  void mobileMessageError();

  void parseMessageError();
  void parseServiceMessageError();

private:
  QNetworkReply* sendRequest(const char* type);

  template <typename ParseCallback>
  void parseMessageXml(const QByteArray& xmlString, const ParseCallback& callback);

  template <typename ParseCallback>
  void parseServiceMessageXml(const QByteArray& xmlString, const ParseCallback& callback);

private:
  QNetworkAccessManager* manager_;

  struct RequestData
  {
    QString server;
    QString password;
    int id;
    QByteArray hash;
  } cache_;

  struct MobileParams
  {
    QString server;
    bool use;
  } mobileParams_;
};

#endif // NETWORKMANAGER_H
