#pragma once

#include <functional>
#include <optional>
#include <string>
#include "value.hh"

namespace options
{
  enum class OptState
  {
    validated,
    excluded,
    required,
    optional
  };

  struct Option
  {
    explicit Option(std::string str);

    Option* with_description(std::string str);
    Option* with_position(std::size_t pos);
    Option* with_value(Value val);
    Option* is_mandatory();
    Option* require(Option* opt);
    Option* exclude(Option* opt);

    template <typename CbType>
    Option* with_parse_cb(CbType&& cb)
    {
      parse_cb = std::forward<CbType>(cb);
      return this;
    }

    std::string label{};
    std::string desc{};
    std::optional<std::size_t> position{};
    bool value = false;

    std::vector<Option*> required_opt{};
    std::vector<Option*> excluded_opt{};
    std::vector<Value> values{};

    OptState state = OptState::optional;
    std::function<void()> parse_cb{};
  };
} // namespace options
