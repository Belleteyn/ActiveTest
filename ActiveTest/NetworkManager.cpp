#include "NetworkManager.h"

#include <QNetworkRequest>
#include <QUrl>
#include <QCryptographicHash>

NetworkManager::NetworkManager(QObject *parent)
  : QObject(parent)
{
  manager_ = new QNetworkAccessManager(this);
  if (manager_)
  {
    if (!QObject::connect(manager_, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*))))
    {
      qCritical() << "cant connect reply finished signal";
    }
  }
  else
  {
    qCritical() << "cant create QNetworkAccessManager";
  }
}

void NetworkManager::ping()
{
  QSettings settings("settings.ini");
  QString server = settings.value("PingServer/server").toString();
  QString password = settings.value("PingServer/password").toString();
  int id = settings.value("PingServer/id").toInt();

  QNetworkReply* reply = sendRequest("PING", server, password, id);
}

void NetworkManager::replyFinished(QNetworkReply* reply)
{

}

QNetworkReply* NetworkManager::sendRequest(const char* type, const QString& server, const QString& password, int id)
{
  QNetworkRequest request;

  request.setUrl(QUrl(server));
  request.setRawHeader("req", QString(type).toUtf8());
  request.setRawHeader("id", QByteArray(reinterpret_cast<char*>(&id), sizeof(int)));

  QCryptographicHash hash(QCryptographicHash::Md5);
  hash.addData(reinterpret_cast<char*>(&id), sizeof(int));
  hash.addData(password.toUtf8());
  hash.addData(reinterpret_cast<char*>(&id), sizeof(int));

  request.setRawHeader("signature", hash.result());

  return manager_->post(request, QByteArray());
}
