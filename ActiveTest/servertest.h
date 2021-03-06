#ifndef SERVERTEST_H
#define SERVERTEST_H

#include <QObject>
#include <QTime>

class ServerTest : public QObject
{
  Q_OBJECT

public:
  explicit ServerTest(QObject *parent = 0);
  ~ServerTest() {}

  void emptyXmlRequest() { emptyXml(); }
  void messageSetConfirm() {}
  void userMessageRequest();
  void serviceMessageRequest();

signals:
  void emptyXml();
  void emptyMessageXml();
  void serviceMessage(long id, const QByteArray& message, const QTime& time);
  void userMessage(long id, const QByteArray& message, const QTime& time, int priority);

private:
  bool randomBool();
  long randomId();
  int randomPrior();
  QByteArray randomMessage();

private:
  int messageIdCounter_;
  QMap<qint64, QByteArray> messageMap_;
};

#endif // SERVERTEST_H
