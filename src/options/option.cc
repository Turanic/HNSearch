#include "option.hh"

namespace options
{
  Option::Option(std::string str)
      : label{std::move(str)}
  {
  }

  Option* Option::with_description(std::string str)
  {
    desc = std::move(str);
    return this;
  }

  Option* Option::with_position(std::size_t pos)
  {
    position = pos;
    return this;
  }

  Option* Option::with_value(Value val)
  {
    values.emplace_back(std::move(val));
    return this;
  }

  Option* Option::is_mandatory()
  {
    state = OptState::required;
    return this;
  }

  Option* Option::require(Option* opt)
  {
    required_opt.emplace_back(opt);
    return this;
  }

  Option* Option::exclude(Option* opt)
  {
    opt->excluded_opt.emplace_back(this);
    excluded_opt.emplace_back(opt);
    return this;
  }

} // namespace options
