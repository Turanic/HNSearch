#pragma once

#include "node.hh"

namespace trie
{
  class Distinct
  {
  public:
    Distinct(const std::vector<node>* nodes);

    Distinct(const Distinct&) = delete;
    const Distinct& operator=(const Distinct&) = delete;
    Distinct(Distinct&&) = delete;
    const Distinct& operator=(Distinct&&) = delete;

    using result_t = unsigned;
    result_t result_get() const;
    void operator()(index_t node_idx);

  private:
    const std::vector<node>* nodes_ = nullptr;
    unsigned distinct_queries_ = 0;
  };
} // namespace trie
