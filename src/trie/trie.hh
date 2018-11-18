#pragma once

#include <deque>
#include <string_view>
#include <vector>

namespace trie
{
  class Trie
  {
  public:
    using index_t = unsigned int;

    Trie();

    void emplace(std::string_view element);
    std::size_t size() const;
    unsigned get_distinct_queries() const;

  private:
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

    template <typename InputIt>
    unsigned insert_node(index_t root_idx, InputIt start, InputIt end);

    template <typename InputIt>
    std::string_view push_to_memory(InputIt start, InputIt end);

    std::vector<node> nodes_{};

    unsigned distinct_queries_ = 0;
    std::deque<std::string> memory_{};
  };
} // namespace trie

#include "trie.hxx"
