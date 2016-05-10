#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <Boss.h>
#include <SystemTray.h>

int main(int argc, char *argv[])
{
  QGuiApplication a(argc, argv);

  Boss boss;
  if (boss.init())
  {
    QQmlApplicationEngine engine;

    SystemTray systemTray;

    engine.rootContext()->setContextProperty("boss", &boss);
    engine.rootContext()->setContextProperty("systemTray", &systemTray);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return a.exec();
  }

  return a.quit();
}
