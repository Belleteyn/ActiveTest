#ifndef NETWORKMANAGERWITHSERVEREMULATION_H
#define NETWORKMANAGERWITHSERVEREMULATION_H

#include <functional>

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <Message.h>

enum class OpResult { Success, EmptyData, ParseError, RequestError };

template <typename ...Args>
using Callback = std::function<void(const OpResult& error, const Args& ... args)>;

class NetworkManagerWithServerEmulation : public QObject
{
  Q_OBJECT
public:
  explicit NetworkManagerWithServerEmulation(QObject *parent = 0);

public:
  void emptyXmlRequest(const Callback<>& callback);
  void userMessageRequest(const Callback<const Message&>& callback);
  void messageSetConfirm(long id, const Callback<>& callback);
  void serviceMessageRequest(const Callback<const Message&>& callback);

  void sendMessageToMobile(const Message& message);
  void sendServiceMessageToMobile(const Message& message);

private:
  bool randomBool();
  long randomId();
  int randomPrior();
  QByteArray randomMessage();

private:
  int messageIdCounter_;
  QMap<qint64, QByteArray> messageMap_;
  bool serverAlive_;
};

#endif // NETWORKMANAGERWITHSERVEREMULATION_H
