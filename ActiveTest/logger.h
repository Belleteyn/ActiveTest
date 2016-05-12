#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QTextStream>
#include <QDateTime>

class Logger
{
public:
  static void compactOutput(QtMsgType type, const QMessageLogContext&, const QString& msg)
  {
    QByteArray data;

    data.append("<" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzzz") + ">");
    data.append("\t");

    QByteArray localMsg = msg.toLocal8Bit();

    switch (type)
    {
    case QtDebugMsg:
      data.append("Debug:\t");
      fprintf(stderr, "Debug: %s\n", localMsg.constData());
      data.append(QString(msg) + "\n");
      Logger::instance().add(data);
      break;
    case QtWarningMsg:
      data.append("Warning:\t");
      fprintf(stderr, "Warning: %s\n", localMsg.constData());
      data.append(QString(msg) + "\n");
      Logger::instance().forceWrite(data);
      break;
    case QtCriticalMsg:
      data.append("Critical:\t");
      fprintf(stderr, "Critical: %s\n", localMsg.constData());
      data.append(QString(msg) + "\n");
      Logger::instance().forceWrite(data);
      break;
    case QtFatalMsg:
      data.append("Fatal:\t");
      fprintf(stderr, "Fatal: %s\n", localMsg.constData());
      data.append(QString(msg) + "\n");
      Logger::instance().forceWrite(data);
    }
  }

  static void fullOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
  {
    QByteArray data;
    data.append("<" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzzz") + ">");
    data.append("\t");

    QByteArray localMsg = msg.toLocal8Bit();
    switch (type)
    {
    case QtDebugMsg:
      data.append("Debug:\t");
      fprintf(stderr, "Debug: %s\n", localMsg.constData());
      data.append(QString("%1:\t\t(%2)\t\t~").arg(context.function).arg(context.line));
      data.append(QString(msg) + "\n");
      Logger::instance().add(data);
      break;
    case QtWarningMsg:
      data.append("Warning:\t");
      fprintf(stderr, "Warning: %s\n", localMsg.constData());
      data.append(QString("%1:\t\t(%2)\t\t~").arg(context.function).arg(context.line));
      data.append(QString(msg) + "\n");
      Logger::instance().forceWrite(data);
      break;
    case QtCriticalMsg:
      data.append("Critical:\t");
      fprintf(stderr, "Critical: %s\n", localMsg.constData());
      data.append(QString("%1:\t\t(%2)\t\t~").arg(context.function).arg(context.line));
      data.append(QString(msg) + "\n");
      Logger::instance().forceWrite(data);
      break;
    case QtFatalMsg:
      data.append("Fatal:\t");
      fprintf(stderr, "Fatal: %s\n", localMsg.constData());
      data.append(QString("%1:\t\t(%2)\t\t~").arg(context.function).arg(context.line));
      data.append(QString(msg) + "\n");
      Logger::instance().forceWrite(data);
    }
  }

public:
  static Logger& instance()
  {
    static Logger loggerInstance;
    return loggerInstance;
  }

  bool open(const QString& name)
  {
    file_.setFileName(name);

    initialized_ = file_.open(QIODevice::WriteOnly);

    if (initialized_)
    {
      stream_.setDevice(&file_);

      qDebug() << "open log";
    }

    return initialized_;
  }

  void close()
  {
    if (initialized_)
    {
      qDebug() << "close log";

      stream_.flush();
      file_.close();

      initialized_ = false;
    }
  }

private:
  Logger() : initialized_(false) {}
  ~Logger() { close(); }

  Logger(const Logger&) = delete;
  Logger& operator = (const Logger&) = delete;

  template <class T>
  inline void add(const T& data)
  {
    if (initialized_)
    {
      stream_ << data;
    }
  }

  template <class T>
  inline void forceWrite(const T& data)
  {
    if (initialized_)
    {
      stream_ << data;

      stream_.flush();
      file_.close();

      initialized_ = file_.open(QIODevice::Append);
    }
  }

private:
  QFile file_;
  QTextStream stream_;

  bool initialized_;
};

#endif // LOGGER_H