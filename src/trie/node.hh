#pragma once

#include <vector>

namespace trie
{
  /* index_t represent the address in the nodes vector.
   * pointers are not used because the vector reallocation
   * invalidated addresses in memory. index are easier and
   * safer to use */
  using index_t = unsigned int;

  /* represent a link between two nodes.
   * value_ptr/_len is a reference to a part of a chunk.
   * child_idx is the index of the child node.
   *
   * Remark: a string_view could be used here but takes more
   * memory (In fact, the struct would size 8 bytes more) */
  struct edge
  {
    const char* value_ptr;
    unsigned value_len;
    index_t child_idx;

    edge(const char* ptr, unsigned len, index_t idx)
        : value_ptr{ptr}
        , value_len{len}
        , child_idx{idx}
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
