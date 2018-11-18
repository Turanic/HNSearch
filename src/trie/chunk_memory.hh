#pragma once

#include <deque>
#include <string>
#include <string_view>

namespace trie
{
  class ChunkMemory
  {
  public:
    template <typename InputIt>
    std::string_view push(InputIt start, InputIt end);

    std::size_t size();

  private:
    /* deque prevent relocation of strings. */
    std::deque<std::string> chunks_{};
  };
} // namespace trie

#include "chunk_memory.hxx"
