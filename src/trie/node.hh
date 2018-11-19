#pragma once

#include <string_view>
#include <vector>

namespace trie
{
  /* index_t represent the address in the nodes vector.
   * pointers are not used because the vector reallocation
   * invalidated addresses in memory. index are easier and
   * safer to use */
  using index_t = unsigned int;

  /* represent a link between two nodes.
   * value is a reference to a part of a chunk.
   * child_idx is the index of the child node */
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

  /* a node of the trie */
  struct node
  {
    index_t parent = 0;    /* index of the parent node */
    unsigned int freq = 0; /* 0 mean it's not a word. another value is the
                              number of time the word appeared */
    std::vector<edge> children{}; /* vector containing edges linking this node
                                     to its children */
  };
} // namespace trie
