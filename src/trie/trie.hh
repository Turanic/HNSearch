#pragma once

#include <string_view>
#include <vector>
#include "chunk_memory.hh"
#include "dummy.hh"
#include "node.hh"

namespace trie
{
  template <typename Command = Dummy>
  class Trie
  {
  public:
    Trie();
    template <typename... Args>
    Trie(Args&&... args);

    void emplace(std::string_view element);
    std::size_t size() const;
    unsigned get_distinct_queries() const;

    typename Command::result_t command_get_result() const;

  private:
    template <typename InputIt>
    index_t insert_node(index_t root_idx, InputIt start, InputIt end);

    template <typename InputIt>
    index_t create_new_child(index_t root_idx, InputIt start, InputIt end);

    template <typename InputIt>
    index_t break_edge(index_t root_idx,
                       edge* edge_ptr,
                       InputIt break_loc,
                       InputIt start,
                       InputIt end);

    std::vector<node> nodes_{};
    ChunkMemory chunks_{};

    Command cmd_;
  };
} // namespace trie

#include "trie.hxx"
