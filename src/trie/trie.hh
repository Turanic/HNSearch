#pragma once

#include <string_view>
#include <vector>
#include "chunk_memory.hh"

namespace trie
{
  class Trie
  {
  public:
    using index_t = unsigned int;

    Trie(unsigned top_limit = 3);

    void emplace(std::string_view element);
    std::size_t size() const;
    unsigned get_distinct_queries() const;
    std::vector<std::pair<std::string, unsigned>> get_top_queries() const;

  private:
    template <typename InputIt>
    index_t insert_node(index_t root_idx, InputIt start, InputIt end);

    struct edge
    {
      std::string_view value;
      index_t child_idx;

      edge(std::string_view v, index_t c)
          : value{v}
          , child_idx{c}
      {
      }
    };

    struct node
    {
      index_t parent = 0;
      unsigned int freq = 0; /* 0 mean it's not a word */
      std::vector<edge> children{};
    };

    std::vector<node> nodes_{};
    ChunkMemory chunks_{};

    /* distinct counter */
    unsigned distinct_queries_ = 0;

    /* top list */
    std::vector<index_t> top_queries_{};
    unsigned top_limit_ = 3;
  };
} // namespace trie

#include "trie.hxx"
