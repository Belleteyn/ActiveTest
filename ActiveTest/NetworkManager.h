#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class NetworkManager : public QObject
{
  Q_OBJECT
public:
  explicit NetworkManager(QObject *parent = 0);

public:
  inline void emptyXmlRequest() { ping(); }
  inline void messageSetConfirm(long id) { messageShown(id); }
  inline void userMessageRequest() { getMessages(); }
  inline void serviceMessageRequest() { getServiceMessage(); }

  void ping();
  void getMessages();
  void messageShown(long id);
  void getServiceMessage();

  void sendMessageToMobile(const QByteArray& urlentext, long id, int priority, const QString& phone, const QTime& time);
  void sendMessageToMobile(const QString& text, long id, int priority, const QString& phone, const QTime& time);

  // не уверен надо ли это
  void sendServiceMessageToMobile(const QByteArray& urlentext, const QTime& time);
  void sendServiceMessageToMobile(const QString& text, const QTime& time);

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
  void parseMessageXml(const QByteArray& xmlString);
  void parseServiceMessageXml(const QByteArray& xmlString);

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
