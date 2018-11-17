#pragma once

#include <memory>
#include <vector>
#include "cmd_line.hh"
#include "option.hh"

namespace options
{
  class OptionsParser
  {
  public:
    OptionsParser(const OptionsParser&) = delete;
    OptionsParser(OptionsParser&&) = delete;
    OptionsParser& operator=(const OptionsParser&) = delete;
    OptionsParser& operator=(OptionsParser&&) = delete;

    template <typename... Args>
    explicit OptionsParser(Args&&... args);

    template <typename Arg>
    Option* add_option(Arg&& arg);

    void parse();

  private:
    bool assign_value(std::string_view* val, std::size_t cmd_pos, ValPos pos);
    void validate_option(Option& opt, std::size_t cmd_pos);

    std::vector<std::unique_ptr<Option>> options_{};
    CmdLine cmd_;
  };
} // namespace options

#include "parser.hxx"
