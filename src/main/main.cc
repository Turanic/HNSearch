#include <cassert>
#include <charconv>
#include <iostream>
#include "options/parser.hh"
#include "tools/benchmark.hh"
#include "tools/logger.hh"
#include "tools/string_conversion.hh"
#include "tools/tsv_parser.hh"
#include "trie/distinct.hh"
#include "trie/top_list.hh"
#include "trie/trie.hh"

namespace
{
  enum class mode
  {
    failure,
    distinct,
    top
  };

  /* Use the custom options parser to parse the command line.
   * It will first define the command line possible options,
   * then it will parse the command line and finally return
   * the important values extracted from the command line. */
  auto parse_command_line(int argc, char* argv[])
  {
    options::OptionsParser opts{argc, argv};

    mode current_mode = mode::failure;

    std::string_view file{};               // file name
    std::string_view nb_queries{};         // number of queries in top mode
    std::string_view ts_from = "0";        // minimum timestamp
    std::string_view ts_to = "2147483647"; // maximum timestamp

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

#ifndef NDEBUG
    /* log and bench are two customs options I used in debug mode to
     * active logging or benchmarking features. In release mode, the
     * logging/benchmarks are not generated so those options are
     * useless */
    {
      using namespace tools;
      opts.add_option("--log")->with_parse_cb(
        [] { Logger::log_get().enable(); });
      opts.add_option("--bench")->with_parse_cb(
        [] { Benchmark::benchmark_get().enable(); });
    }
#endif

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

  /* This function will extract all words from parser prs and insert those
   * in the Trie words. It will then output the result of the trie command. */
  template <typename Command>
  void parse_datas(trie::Trie<Command>& words, tools::TSVParser& prs)
  {
    BENCH_START(bench_1, "all insertion");
    while (true)
    {
      auto [tm, elt, eof] = prs.read_element();
      if (eof)
        break;

      assert(not elt.empty());

      LOG("processing string %s\n", elt.c_str());
      words.emplace(elt);
    }
    BENCH_STOP(bench_1);
    LOG("Final tree is composed of %lu nodes\n", words.size());

    BENCH_START(bench_2, "result extraction");
    while (true)
    if constexpr (std::is_same_v<Command, trie::TopList>)
    {
      /* TopList command yield a vector of the most frequent words */
      auto results = words.command_get_result();
      for (const auto& [word, freq] : results)
        std::cout << word << " " << freq << '\n';
      return;
    }

    if constexpr (std::is_same_v<Command, trie::Distinct>)
    {
      /* Distinct command yield the number of distinct words */
      std::cerr << words.command_get_result() << '\n';
      return;
    }
  }
} // namespace

int main(int argc, char* argv[]) try
{
  auto [file, nb_queries, ts_from, ts_to, cmode] =
    parse_command_line(argc, argv);

  LOG("file: %s; nb_queries: %s; range: [%s,%s]\n",
      file.data(),
      nb_queries.data(),
      ts_from.data(),
      ts_to.data());

  tools::TSVParser prs{file.data(), ts_from, ts_to};
  switch (cmode)
  {
  case mode::distinct:
  {
    trie::Trie<trie::Distinct> trie{};
    parse_datas(trie, prs);
    break;
  }
  case mode::top:
  {
    trie::Trie<trie::TopList> trie{tools::string_to<unsigned>(nb_queries)};
    parse_datas(trie, prs);
    break;
  }
  default:
    break;
  }

  return 0;
}
catch (const std::exception& e)
{
  std::cerr << e.what() << '\n';
  return 1;
}
