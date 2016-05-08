#ifndef SERVERTEST_H
#define SERVERTEST_H

#include <QObject>

class ServerTest : public QObject
{
  Q_OBJECT

public:
  explicit ServerTest(QObject *parent = 0);
  ~ServerTest() {}

  void emptyXmlRequest() { emptyXml(); }
  void messageSetConfirm() {}
  void userMessageRequest() { userMessage(); }
  void serviceMessageRequest() { serviceMessage(); }

signals:
  void emptyXml();
  void emptyMessageXml();
  void serviceMessage();
  void userMessage();
};

#endif // SERVERTEST_H
