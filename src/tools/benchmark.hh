#pragma once

#include <chrono>
#include <memory>
#include <unordered_map>

/* Custom library to micro benchmark some part of the code.
 * BENCH_START give you an object. Its lifetime should correspond
 * to the code you would like to benchmark. BENCH_STOP can be used
 * to directly end the recording time of a benchmark.
 *
 * Benchmarks are displayed at the end of the program.
 * Remark: Benchmark macro are disabled on release builds. */
#ifndef NDEBUG
#define BENCH_START(name, bench)                                               \
  auto name = tools::Benchmark::benchmark_get().start_bench(bench)
#define BENCH_STOP(name)                                                       \
  do                                                                           \
  {                                                                            \
    if (name != nullptr)                                                       \
    {                                                                          \
      name.reset();                                                            \
    }                                                                          \
  } while (0)
#else
#define BENCH_START(name, bench)                                               \
  do                                                                           \
  {                                                                            \
  } while (0)
#define BENCH_STOP(name)                                                       \
  do                                                                           \
  {                                                                            \
  } while (0)
#endif

namespace tools
{
  class Benchmark
  {
  public:
    static auto& benchmark_get()
    {
      static Benchmark instance{};
      return instance;
    }

    void enable();

    struct Timer;
    struct Stat;
    std::unique_ptr<Timer> start_bench(const std::string& name);

  private:
    std::unordered_map<std::string, std::unique_ptr<Stat>> benchs_{};

    bool enabled_ = false;

    Benchmark() = default;
    ~Benchmark();
  };

  struct Benchmark::Stat
  {
    std::chrono::microseconds dur{0};
    unsigned int nb_run = 0;
  };

  class Benchmark::Timer
  {
  public:
    Timer(Stat* ptr)
        : start{std::chrono::system_clock::now()}
        , stats{ptr}
    {
    }

    ~Timer()
    {
      stats->dur += std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now() - start);
      stats->nb_run++;
    }

  private:
    std::chrono::system_clock::time_point start;
    Stat* stats;
    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
  };
} // namespace tools
