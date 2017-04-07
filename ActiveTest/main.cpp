#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>
#include <QDir>
#include <QDebug>

#include <spdlog/spdlog.h>
#include <LogHelper.h>
#include <HandledSink.h>

#include <Boss.h>
#include <SystemTray.h>

int main(int argc, char *argv[])
{
  QGuiApplication a(argc, argv);

  QTranslator translator;
  translator.load("activetest_ru");
  a.installTranslator(&translator);

  QObject::connect(&a, &QGuiApplication::aboutToQuit, []()
  {
    Loggers::app->info() << "application stopped";
  });

  QString logDirName("SMSLogs");
  QDir logDir(QGuiApplication::applicationDirPath());
  if (!logDir.exists(logDirName))
  {
    logDir.mkpath(logDirName);
  }
  logDir.cd(logDirName);

  QString logPath = logDir.absolutePath() + "/log";

  Boss boss;

  try
  {
    std::vector<spdlog::sink_ptr> sinks;

#ifdef LOG_DEBUG
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
#endif
    sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(logPath.toStdString(), "txt", 7, 00, true));

    sinks.push_back(std::make_shared<handled_sink_mt>([&boss](const std::string& tag, const std::string& message)
    {
      boss.appendLogString(QString::fromStdString(tag), QString::fromStdString(message));
    }));

    Loggers::net = spdlog::create("net", std::begin(sinks), std::end(sinks));
    Loggers::sms = spdlog::create("sms", std::begin(sinks), std::end(sinks));
    Loggers::app = spdlog::create("app", std::begin(sinks), std::end(sinks));

#ifdef LOG_DEBUG
		spdlog::set_level(spdlog::level::trace);
#else
		spdlog::set_level(spdlog::level::info);
#endif

    if (!(Loggers::net && Loggers::sms && Loggers::app))
    {
      qFatal("log creation failed");
    }

#ifdef LOG_DEBUG
    spdlog::set_pattern("[%T.%e] [%n] [%l] [thread: %t] %v");
#elif LOG_RELEASE
    spdlog::set_pattern("[%Y-%m-%d %T.%e] [%n] %v");
#endif
  }
  catch (const spdlog::spdlog_ex&)
  {
    qFatal("log creation failed");
  }

  if (boss.init())
  {
    QQmlApplicationEngine engine;

    SystemTray systemTray;

    engine.rootContext()->setContextProperty("boss", &boss);
    engine.rootContext()->setContextProperty("systemTray", &systemTray);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    Loggers::app->info() << "application started";

    return a.exec();
  }

  a.quit();
  return -1;
}
