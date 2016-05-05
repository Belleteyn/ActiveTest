#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJSValue>
#include <QSettings>

class NetworkManager : public QObject
{
  Q_OBJECT
public:
  explicit NetworkManager(QObject *parent = 0);

public:
  Q_INVOKABLE void ping();

signals:

public slots:
private slots:
  void replyFinished(QNetworkReply* reply);

private:
  QNetworkReply* sendRequest(const char* type, const QString& server, const QString& password, int id);

public:
  QNetworkAccessManager* manager_;
};

#endif // NETWORKMANAGER_H
