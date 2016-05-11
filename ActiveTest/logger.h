#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QTextStream>
#include <QDateTime>

class Logger;
Logger* instance = nullptr;

class Logger
{
public:
  Logger()
  {
    instance = this;
  }
  ~Logger()
  {
    qDebug() << "close application";
    logFile.close();
    instance = nullptr;
  }

  bool initLog(const QString& name)
  {
    logFile.setFileName(name);
    if (logFile.open(QIODevice::WriteOnly))
    {
      ts.setDevice(&logFile);
    }
    return false;
  }

  static void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
  {
    QByteArray data;
    data.append("<" + QDateTime::currentDateTime().toString("yyyy-MMMM-dd hh:mm:ss.zzzz") + ">");
    data.append("\t");
    data.append(QString("%1:\t\t(%2)\t\t~").arg(context.function).arg(context.line));

    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
      data.append("Debug:\t");
      fprintf(stderr, "Debug: %s\n", localMsg.constData());
      break;
    case QtWarningMsg:
      data.append("Warning:\t");
      fprintf(stderr, "Warning: %s\n", localMsg.constData());
      break;
    case QtCriticalMsg:
      data.append("Critical:\t");
      fprintf(stderr, "Critical: %s\n", localMsg.constData());
      break;
    case QtFatalMsg:
      data.append("Fatal:\t");
      fprintf(stderr, "Fatal: %s\n", localMsg.constData());
    }
    data.append(QString(msg) + "\n");

    if (instance)
    {
      instance->ts << data;
    }
  }

private:
  QFile logFile;
  QTextStream ts;
};

#endif // LOGGER_H
