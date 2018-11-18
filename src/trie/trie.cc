#include "trie.hh"
#include <algorithm>
#include "tools/logger.hh"
#include "tools/benchmark.hh"

namespace trie
{
  Trie::Trie()
  {
    /* create root node */
    nodes_.resize(1);
    // TODO: reserve ?

    LOG("sizeof(node) = %lu\n", sizeof(node));
    LOG("sizeof(edge) = %lu\n", sizeof(edge));
  }

  void Trie::emplace(std::string_view str)
  {
    if (str.empty())
      return;

    BENCH_START(bench_insert, "node insertion");
    auto freq = insert_node(0, str.begin(), str.end());
    distinct_queries_ += freq == 1;
  }
} // namespace trie
