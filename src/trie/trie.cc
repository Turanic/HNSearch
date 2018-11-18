#include "trie.hh"
#include <cassert>
#include "tools/benchmark.hh"
#include "tools/logger.hh"

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
    assert(freq > 0);

    distinct_queries_ += freq == 1;
  }
} // namespace trie
