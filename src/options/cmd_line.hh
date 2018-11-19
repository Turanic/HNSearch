#pragma once

#include <string_view>
#include <vector>

namespace options
{
  /* Simple representation of the command line.
   * In short, this is a wrapper around the standard char*[]. */
  class CmdLine
  {
  private:
    std::vector<std::pair<std::string_view, bool>> args_{};

  public:
    CmdLine(int argc, char** argv);

    std::pair<std::string_view, bool>& operator[](std::size_t idx);
    const std::pair<std::string_view, bool>& operator[](std::size_t idx) const;

    using const_iterator = decltype(args_)::const_iterator;
    using iterator = decltype(args_)::iterator;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    iterator begin() noexcept;
    iterator end() noexcept;
    iterator erase(const_iterator it);
    std::size_t size() const noexcept;
  };
} // namespace options
