#include "distinct.hh"

namespace trie
{
  Distinct::Distinct(const std::vector<node>* nodes)
      : nodes_{nodes}
  {
  }

  void Distinct::operator()(index_t node_idx)
  {
    auto freq = nodes_->at(node_idx).freq;
    distinct_queries_ += freq == 1;
  }

  Distinct::result_t Distinct::result_get() const
  {
    return distinct_queries_;
  }

} // namespace trie
