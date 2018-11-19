#include "parser.hh"
#include <cassert>
#include <stdexcept>

namespace options
{
  void OptionsParser::parse()
  {
    for (auto& option : options_)
    {
      if (option->state == OptState::excluded)
        continue;

      if (option->position.has_value())
      {
        validate_option(*option, *option->position);
        continue;
      }

      auto cmd_itr =
        std::find_if(cmd_.cbegin(), cmd_.cend(), [&option](const auto& elt) {
          return option->label == elt.first && not elt.second;
        });
      validate_option(*option, std::distance(cmd_.cbegin(), cmd_itr));
    }

    for (const auto& option : options_)
    {
      if (option->state == OptState::required)
        throw std::runtime_error{"option required"};
      else if (option->state == OptState::validated && option->parse_cb)
        option->parse_cb();
    }

    for (const auto& [str, validated] : cmd_)
    {
      if (not validated)
        throw std::runtime_error{std::string{str.data(), str.size()}};
    }
  }

  bool OptionsParser::assign_value(std::string_view* val,
                                   std::size_t cmd_pos,
                                   ValPos pos)
  {
    std::size_t val_pos = 0;
    bool out = true;
    switch (pos)
    {
    case ValPos::after:
      val_pos = cmd_pos + 1;
      out = cmd_pos >= cmd_.size() - 1;
      break;
    case ValPos::before:
      val_pos = cmd_pos - 1;
      out = cmd_pos == 0;
      break;
    case ValPos::last:
      val_pos = cmd_.size() - 1;
      out = cmd_pos == cmd_.size();
      break;
    default:
      break;
    };

    if (out)
      return false;

    const auto& [str, validated] = cmd_[val_pos];
    if (validated)
      return false;

    *val = str;
    cmd_[val_pos].second = true;
    return true;
  }

  void OptionsParser::validate_option(Option& opt, std::size_t cmd_pos)
  {
    if (cmd_pos >= cmd_.size())
      return;

    const auto& [str, validated] = cmd_[cmd_pos];
    assert(not validated);

    if (str != opt.label)
      return;

    for (auto& val : opt.values)
      if (not assign_value(val.value, cmd_pos, val.position))
        return;

    for (auto opt_ptr : opt.excluded_opt)
      opt_ptr->state = OptState::excluded;

    for (auto opt_ptr : opt.required_opt)
      opt_ptr->state = OptState::required;

    opt.state = OptState::validated;
    cmd_[cmd_pos].second = true;
  }
} // namespace options
