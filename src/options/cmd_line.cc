#include "cmd_line.hh"
#include <algorithm>

namespace options
{
  CmdLine::CmdLine(int argc, char** argv)
  {
    args_.reserve(argc);
    for (int i = 0; i < argc; ++i)
      args_.emplace_back(argv[i]);
  }

  std::string_view CmdLine::at(std::size_t idx)
  {
    return args_.at(idx);
  }

  CmdLine::const_iterator CmdLine::erase(const_iterator it)
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

  std::size_t CmdLine::size() const noexcept
  {
    return args_.size();
  }
} // namespace options
