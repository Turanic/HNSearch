#include "options/parser.hh"

#include <iostream>

enum class mode
{
  failure,
  distinct,
  top
};

namespace
{
  auto parse_command_line(int argc, char* argv[])
  {
    options::OptionsParser opts{argc, argv};

    mode current_mode = mode::failure;

    std::string_view file{};
    std::string_view nb_queries{};
    std::string_view ts_from = "0";
    std::string_view ts_to = "2147483647";

    auto opt_distinct = opts.add_option("distinct");
    opt_distinct->with_position(1)
      ->with_value({&file, options::ValPos::last})
      ->is_mandatory()
      ->with_parse_cb([&current_mode] { current_mode = mode::distinct; });

    auto opt_top = opts.add_option("top");
    opt_top->with_position(1)
      ->is_mandatory()
      ->with_value({&file, options::ValPos::last})
      ->with_value({&nb_queries, options::ValPos::after})
      ->exclude(opt_distinct)
      ->with_parse_cb([&current_mode] { current_mode = mode::top; });

    auto opt_ts_from = opts.add_option("--from");
    opt_ts_from->with_value({&ts_from, options::ValPos::after});

    auto opt_ts_to = opts.add_option("--to");
    opt_ts_to->with_value({&ts_to, options::ValPos::after});

    try
    {
      opts.parse();
    }
    catch (...)
    {
      std::cerr
        << "Usage:\n"
        << "  ./hnStat distinct [--from TIMESTAMP] [--to TIMESTAMP] "
           "input_file\n"
        << "  ./hnStat top nb_queries [--from TIMESTAMP] [--to TIMESTAMP] "
           "input_file\n";
      throw;
    }

    return std::make_tuple(file, nb_queries, ts_from, ts_to, current_mode);
  }
} // namespace

int main(int argc, char* argv[])
{
  auto [file, nb_queries, ts_from, ts_to, cmode] =
    parse_command_line(argc, argv);

  std::cout << "file: " << file << "\nnb_queries: " << nb_queries
            << "\nrange: [" << ts_from << "," << ts_to << "]\n";

  return 0;
}
