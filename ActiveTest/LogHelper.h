#ifndef LOGHELPER_H
#define LOGHELPER_H

#include <QString>

#include <spdlog/spdlog.h>

namespace Loggers
{
  extern std::shared_ptr<spdlog::logger> app;

  extern std::shared_ptr<spdlog::logger> sms;
  extern std::shared_ptr<spdlog::logger> net;
}

// stream operators

std::ostream& operator<<(std::ostream& os, const QString& qString);

#endif // LOGHELPER_H
