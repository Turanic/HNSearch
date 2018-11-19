#pragma once

#include <string_view>
#include <vector>

namespace trie
{
  using index_t = unsigned int;

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
} // namespace trie
