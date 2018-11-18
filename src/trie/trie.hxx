#pragma once
#include <algorithm>
#include "trie.hh"

namespace trie
{
  template <typename InputIt>
  std::string_view Trie::push_to_memory(InputIt start, InputIt end)
  {
    return memory_.emplace_back(start, end);
  }

  template <typename InputIt>
  unsigned Trie::insert_node(index_t root_idx, InputIt start, InputIt end)
  {
    auto& root_node = nodes_.at(root_idx);
    auto& children = root_node.children;

    if (start == end)
      return ++root_node.freq;

    auto child_iter =
      std::find_if(children.begin(), children.end(), [start](const auto& elt) {
        return *start == elt.value.front();
      });

    /* First case: Completly new fragment */
    if (child_iter == children.end())
    {
      auto new_idx = size();
      children.emplace_back(push_to_memory(start, end), new_idx);
      auto& new_node = nodes_.emplace_back(node{});
      new_node.parent = root_idx;

      return ++new_node.freq;
    }

    const auto& str = child_iter->value;
    auto [new_start, sit] = std::mismatch(start, end, str.begin(), str.end());

    /* Second case: Insert a new node between this one and the child */
    if (sit != str.end())
    {;
      auto off = std::distance(str.begin(), sit);
      std::string_view first_chunk(str.data(), off);
      std::string_view right_chunk(str.data() + off, str.size() - off);
      std::string_view left_chunk(push_to_memory(new_start, end));
      auto old_child_idx = child_iter->child_idx;

      /* create the new intermediate node*/
      auto inter_idx = size();
      child_iter->value = std::move(first_chunk);
      child_iter->child_idx = inter_idx;
      nodes_.emplace_back(node{}).parent = root_idx;

      /* create the right son node */
      nodes_[inter_idx].children.emplace_back(std::move(right_chunk),
                                              old_child_idx);
      nodes_[old_child_idx].parent = inter_idx;

      auto left_idx = size();
      if (not left_chunk.empty())
      {
        /* create the left son node */
        nodes_[inter_idx].children.emplace_back(std::move(left_chunk),
                                                left_idx);
        nodes_.emplace_back(node{}).parent = inter_idx;
      }

      return left_chunk.empty() ? ++nodes_[inter_idx].freq
                                : ++nodes_[left_idx].freq;
    }

    return insert_node(child_iter->child_idx, new_start, end);
  }

  inline std::size_t Trie::size() const
  {
    return std::size(nodes_);
  }

  inline unsigned Trie::get_distinct_queries() const
  {
    return distinct_queries_;
  }
} // namespace trie
