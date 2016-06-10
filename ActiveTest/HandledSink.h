#ifndef HANDLEDSINK
#define HANDLEDSINK

#include <functional>
#include <mutex>

#include <spdlog/spdlog.h>

template <class Mutex>
class handled_sink : public spdlog::sinks::base_sink<Mutex>
{
    using StringHandler = std::function<void(const std::string& /*tag*/, const std::string& /*message*/)>;
public:
    handled_sink(StringHandler&& stringHandler) : spdlog::sinks::base_sink<Mutex>(), stringHandler_(stringHandler) {}

    void _sink_it(const spdlog::details::log_msg& msg) override
    {
      stringHandler_(msg.logger_name, msg.raw.str());
    }

    void flush() override {}

private:
  StringHandler stringHandler_;
};

typedef handled_sink<spdlog::details::null_mutex> handled_sink_st;
typedef handled_sink<std::mutex> handled_sink_mt;

#endif // HANDLEDSINK

