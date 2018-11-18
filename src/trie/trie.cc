#include "trie.hh"
#include <cassert>
#include "tools/benchmark.hh"
#include "tools/logger.hh"

namespace trie
{
  Trie::Trie(unsigned top_limit)
      : top_limit_{top_limit}
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
    auto node_idx = insert_node(0, str.begin(), str.end());
    auto freq = nodes_.at(node_idx).freq;
    assert(freq > 0);

    distinct_queries_ += freq == 1;

    if (not top_queries_.empty() && node_idx == top_queries_.back())
      top_queries_.pop_back();

    if (top_queries_.size() >= top_limit_
        && nodes_[top_queries_.back()].freq >= freq)
      return;

    auto inf_it =
      std::find_if(top_queries_.begin(),
                   top_queries_.end(),
                   [this, freq, node_idx](auto top_idx) {
                     return top_idx == node_idx || nodes_[top_idx].freq < freq;
                   });

    auto new_it = top_queries_.emplace(inf_it, node_idx);

    auto del_it = std::find(std::next(new_it), top_queries_.end(), node_idx);
    if (del_it != top_queries_.end())
      top_queries_.erase(del_it);
    else if (top_queries_.size() > top_limit_)
      top_queries_.pop_back();
  }
} // namespace trie
