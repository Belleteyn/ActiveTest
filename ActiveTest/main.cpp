#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QDebug>

#include <spdlog/spdlog.h>

#include <Boss.h>
#include <SystemTray.h>

int main(int argc, char *argv[])
{
  try
  {
    std::vector<spdlog::sink_ptr> sinks;

#ifdef LOG_DEBUG
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
#endif
    sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>("log", "txt", 7, 00, true));

    spdlog::create("net", std::begin(sinks), std::end(sinks));
    spdlog::create("sms", std::begin(sinks), std::end(sinks));
    spdlog::create("app", std::begin(sinks), std::end(sinks));

    spdlog::set_level(spdlog::level::trace);

#ifdef LOG_DEBUG
    spdlog::set_pattern("[%T.%e] [%n] [%l] [thread: %t] %v");
#elif LOG_RELEASE
    spdlog::set_pattern("[%Y-%m-%d %T.%e] [%n] %v");
#endif
  }
  catch (const spdlog::spdlog_ex& ex)
  {
    qDebug() << "Log failed: " << ex.what();
  }

  QGuiApplication a(argc, argv);

  QObject::connect(&a, &QGuiApplication::aboutToQuit, []()
  {
    spdlog::get("app")->info() << "application stopped";
  });

  Boss boss;
  if (boss.init())
  {
    QQmlApplicationEngine engine;

    SystemTray systemTray;

    engine.rootContext()->setContextProperty("boss", &boss);
    engine.rootContext()->setContextProperty("systemTray", &systemTray);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    try
    {
      spdlog::get("app")->info() << "application started";
    }
    catch (const spdlog::spdlog_ex& ex)
    {
      qDebug() << "Log failed: " << ex.what();
    }

    return a.exec();
  }

  a.quit();
  return -1;
}
