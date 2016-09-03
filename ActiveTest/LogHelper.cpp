#include "LogHelper.h"

namespace Loggers
{
  std::shared_ptr<spdlog::logger> app;

  std::shared_ptr<spdlog::logger> sms;
  std::shared_ptr<spdlog::logger> net;
}

std::ostream&operator<<(std::ostream& os, const QString& qString)
{
  std::string stdString = qString.toStdString();
  return os << stdString;
}
