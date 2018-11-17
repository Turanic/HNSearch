#include "logger.hh"
#include <utility>
#include <cstdio>

namespace tools
{
  inline void Logger::enable()
  {
    log_enabled_ = true;
  }

  inline bool Logger::is_enabled() const
  {
    return log_enabled_;
  }

  template <typename... Args>
  void Logger::log(const char* frmt, Args&&... arg) const
  {
    std::printf("(LOG) ");
    std::printf(frmt, std::forward<Args>(arg)...);
  }
} // namespace tools
