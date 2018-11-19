#pragma once

#include <string>
#include "node.hh"

namespace trie
{
  /* TopList command keep track of the most frequent words inserted
   * in a trie */
  class TopList
  {
  public:
    TopList(const std::vector<node>* nodes, unsigned top_limit);

    TopList(const TopList&) = delete;
    const TopList& operator=(const TopList&) = delete;
    TopList(TopList&&) = delete;
    const TopList& operator=(TopList&&) = delete;

    using result_t = std::vector<std::pair<std::string, unsigned>>;
    result_t result_get() const;

    void operator()(index_t node_idx);

  private:
    std::vector<index_t> top_queries_{};
    const std::vector<node>* nodes_ = nullptr;
    unsigned top_limit_ = 3;
  };
} // namespace trie
