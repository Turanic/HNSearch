#pragma once

#include <string_view>
#include <vector>
#include "chunk_memory.hh"
#include "dummy.hh"
#include "node.hh"

namespace trie
{
  /* The trie class is a data container used to efficiently
   * store strings. It can be parameterized with a Command.
   * Commands are used to apply an additional algorithm at
   * each insertion */
  template <typename Command = Dummy>
  class Trie
  {
  public:
    /* Constructors of the Trie. Arguments are forwarded to
     * the underlying command */
    Trie();
    template <typename... Args>
    Trie(Args&&... args);

    /* push a new string in the Trie */
    void emplace(std::string_view element);

    /* return the number of nodes contained in the Trie */
    std::size_t size() const;

    /* get the result of the underlying command */
    typename Command::result_t command_get_result() const;

  private:
    /* helper function the insert a range of characters in the trie with
     * root node at index root_idx */
    template <typename InputIt>
    index_t insert_node(index_t root_idx, InputIt start, InputIt end);

    /* helper function the insert a new child to the node at the index root_idx */
    template <typename InputIt>
    index_t create_new_child(index_t root_idx, InputIt start, InputIt end);

    /* helper function the insert a node between two others. */
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
