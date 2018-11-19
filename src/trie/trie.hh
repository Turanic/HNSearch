#pragma once

#include <string_view>
#include <vector>
#include "chunk_memory.hh"
#include "node.hh"

namespace trie
{
  template <typename Command>
  class Trie
  {
  public:
    Trie();
    template <typename... Args>
    Trie(Args&&... args);

    void emplace(std::string_view element);
    std::size_t size() const;
    unsigned get_distinct_queries() const;
    std::vector<std::pair<std::string, unsigned>> get_top_queries() const;

    typename Command::result_t command_get_result() const;

  private:
    template <typename InputIt>
    index_t insert_node(index_t root_idx, InputIt start, InputIt end);

    std::vector<node> nodes_{};
    ChunkMemory chunks_{};

    Command cmd_;

    /* distinct counter */
    unsigned distinct_queries_ = 0;
  };
} // namespace trie

#include "trie.hxx"
