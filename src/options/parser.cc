#include "parser.hh"
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
        validate_option(*option, option->position.value());
        continue;
      }

      auto cmd_itr = std::find(cmd_.cbegin(), cmd_.cend(), option->label);
      validate_option(*option, std::distance(cmd_.cbegin(), cmd_itr));
    }

    for (const auto& option : options_)
    {
      if (option->state == OptState::required)
        throw std::runtime_error{"option required"};
      else if (option->state == OptState::validated && option->parse_cb)
        option->parse_cb();
    }
  }

  bool OptionsParser::assign_value(std::string_view* val,
                                   std::size_t cmd_pos,
                                   ValPos pos)
  {
    switch (pos)
    {
    case ValPos::after:
    {
      auto val_pos = cmd_pos + 1;
      if (cmd_pos >= cmd_.size() - 1)
        return false;
      *val = cmd_.at(val_pos);
      cmd_.erase(cmd_.cbegin() + val_pos);
      return true;
    }
    case ValPos::before:
    {
      auto val_pos = cmd_pos - 1;
      if (cmd_pos == 0)
        return false;
      *val = cmd_.at(val_pos);
      cmd_.erase(cmd_.cbegin() + val_pos);
      return true;
    }
    case ValPos::last:
    {
      auto val_pos = cmd_.size() - 1;
      if (cmd_pos == cmd_.size())
        return false;
      *val = cmd_.at(val_pos);
      cmd_.erase(cmd_.cbegin() + val_pos);
      return true;
    }
    default:
      return false;
    };
  }

  void OptionsParser::validate_option(Option& opt, std::size_t cmd_pos)
  {
    if (cmd_pos >= cmd_.size())
      return;

    if (cmd_.at(cmd_pos) != opt.label)
      return;

    for (auto& val : opt.values)
      if (not assign_value(val.value, cmd_pos, val.position))
        return;

    for (auto opt_ptr : opt.excluded_opt)
      opt_ptr->state = OptState::excluded;

    for (auto opt_ptr : opt.required_opt)
      opt_ptr->state = OptState::required;

    opt.state = OptState::validated;
  }
} // namespace options
