#include "cmd_line.hh"
#include <algorithm>

namespace options
{
  CmdLine::CmdLine(int argc, char** argv)
  {
    args_.reserve(argc);
    for (int i = 0; i < argc; ++i)
      args_.emplace_back(argv[i], false);

    if (argc > 1) // first argument (program name) should not be used
      args_[0].second = true;
  }

  std::pair<std::string_view, bool>& CmdLine::operator[](std::size_t idx)
  {
    return args_[idx];
  }

  const std::pair<std::string_view, bool>& CmdLine::
  operator[](std::size_t idx) const
  {
    return args_[idx];
  }

  CmdLine::iterator CmdLine::erase(const_iterator it)
  {
    return args_.erase(it);
  }

  CmdLine::const_iterator CmdLine::cbegin() const noexcept
  {
    return args_.cbegin();
  }

  CmdLine::const_iterator CmdLine::cend() const noexcept
  {
    return args_.cend();
  }

  CmdLine::iterator CmdLine::begin() noexcept
  {
    return args_.begin();
  }

  CmdLine::iterator CmdLine::end() noexcept
  {
    return args_.end();
  }

  std::size_t CmdLine::size() const noexcept
  {
    return args_.size();
  }
} // namespace options
