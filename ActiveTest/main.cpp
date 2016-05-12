#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <Boss.h>
#include <SystemTray.h>

#include <QDir>
#include <logger.h>

int main(int argc, char *argv[])
{
#ifdef LOG_DEBUG
  qInstallMessageHandler(&Logger::fullOutput);
#elif LOG_RELEASE
  qInstallMessageHandler(&Logger::compactOutput);
#endif

  QGuiApplication a(argc, argv);

  QString logDirName("SMSLogs");
  QDir logDir(QGuiApplication::applicationDirPath());
  if (!logDir.exists(logDirName))
  {
    logDir.mkpath(logDirName);
  }
  logDir.cd(logDirName);
  QString logFileName = QString(logDir.absolutePath() + "/log_%1.txt")
    .arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh-mm-ss"));

  Logger& logger = Logger::instance();
  logger.open(logFileName);

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
