#include "benchmark.hh"

namespace tools
{
  Benchmark::~Benchmark()
  {
    for (const auto& [name, stat] : benchs_)
    {
      auto avg = stat->dur.count() / stat->nb_run;

      std::printf("(BENCH) %s : %lu us avg on %u tests\n",
                  name.c_str(),
                  avg,
                  stat->nb_run);
    }
  }

  void Benchmark::enable()
  {
    enabled_ = true;
  }

  std::unique_ptr<Benchmark::Timer> Benchmark::start_bench(const std::string& name)
  {
    if (not enabled_)
      return nullptr;

    const auto& ret = benchs_.try_emplace(name, std::make_unique<Stat>());

    return std::make_unique<Timer>(ret.first->second.get());
  }
} // namespace tools
