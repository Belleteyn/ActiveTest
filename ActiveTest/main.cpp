#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QAxObject>
#include <QObject>
#include <QDebug>

#include <Boss.h>

int main(int argc, char *argv[])
{
  QGuiApplication a(argc, argv);

  Boss boss;
  if (boss.init())
  {
    return a.exec();
  }

  return a.quit();

//    QGuiApplication app(argc, argv);

//    SMSObjectManager smsObjectManager;

//    QQmlApplicationEngine engine;

//    engine.rootContext()->setContextProperty("smsObjectManager", &smsObjectManager);

//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

//    return app.exec();
}
