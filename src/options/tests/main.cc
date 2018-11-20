#include <options/parser.hh>

namespace
{
  bool check_simple_parser()
  {
    bool res = false;
    std::string str1 = "./test";
    std::string str2 = "--opt1";
    char* args[] = {str1.data(), str2.data()};
    options::OptionsParser opts{2, args};

    opts.add_option("--opt1")->with_parse_cb([&res] { res = true; });
    opts.add_option("--opt2")->with_parse_cb([&res] { res = false; });

    opts.parse();

    return res;
  }

  bool check_value_assignement()
  {
    std::string_view value{};
    std::string str1 = "./test";
    std::string str2 = "--result";
    std::string str3 = "success";
    char* args[] = {str1.data(), str2.data(), str3.data()};
    options::OptionsParser opts{3, args};

    opts.add_option("--result")->with_value({&value, options::ValPos::after});

    opts.parse();

    return value == "success";
  }
} // namespace

int main()
{
  if (check_simple_parser() && check_value_assignement())
    return 0;

  return 1;
}
