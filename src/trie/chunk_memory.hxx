#pragma once

#include "chunk_memory.hh"

namespace trie
{
  template <typename InputIt>
  std::string_view ChunkMemory::push(InputIt start, InputIt end)
  {
    return chunks_.emplace_back(start, end);
  }

  inline std::size_t ChunkMemory::size()
  {
    return chunks_.size();
  }
} // namespace trie
