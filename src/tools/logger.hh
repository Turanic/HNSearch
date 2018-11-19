#pragma once

/* Custom logging library.
 *
 * Remark: LOG macro is disabled on release builds. */
#ifndef NDEBUG
#define LOG(...)                                                               \
  do                                                                           \
  {                                                                            \
    if (tools::Logger::log_get().is_enabled())                                 \
      tools::Logger::log_get().log(__VA_ARGS__);                               \
  } while (0)
#else
#define LOG(...)                                                               \
  do                                                                           \
  {                                                                            \
  } while (0)
#endif

namespace tools
{
  class Logger
  {
  public:
    static auto& log_get()
    {
      static Logger instance{};
      return instance;
    }

    void enable();
    bool is_enabled() const;

    template <typename... Args>
    void log(const char* frmt, Args&&... arg) const;

  private:
    bool log_enabled_ = false;

    Logger() = default;
  };
} // namespace tools

#include "logger.hxx"
