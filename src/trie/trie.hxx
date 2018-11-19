#pragma once

#include "trie.hh"

#include <algorithm>
#include <cassert>
#include "tools/benchmark.hh"
#include "tools/logger.hh"

namespace trie
{
  template <typename Command>
  template <typename InputIt>
  unsigned
  Trie<Command>::insert_node(index_t root_idx, InputIt start, InputIt end)
  {
    assert(root_idx < nodes_.size());

    auto& root_node = nodes_.at(root_idx);
    auto& children = root_node.children;

    if (start == end)
    {
      ++root_node.freq;
      return root_idx;
    }

    auto child_iter =
      std::find_if(children.begin(), children.end(), [start](const auto& elt) {
        return *start == elt.value.front();
      });

    /* First case: Completly new fragment */
    if (child_iter == children.end())
    {
      auto new_idx = size();
      children.emplace_back(chunks_.push(start, end), new_idx);
      auto& new_node = nodes_.emplace_back(node{});
      new_node.parent = root_idx;
      ++new_node.freq;

      return new_idx;
    }
    assert(child_iter != children.end());

    const auto& str = child_iter->value;
    auto [new_start, sit] = std::mismatch(start, end, str.begin(), str.end());

    /* Second case: Insert a new node between this one and the child */
    if (sit != str.end())
    {
      auto off = std::distance(str.begin(), sit);
      std::string_view first_chunk(str.data(), off);
      std::string_view right_chunk(str.data() + off, str.size() - off);
      std::string_view left_chunk(chunks_.push(new_start, end));
      auto old_child_idx = child_iter->child_idx;

      /* create the new intermediate node*/
      assert(not first_chunk.empty());
      auto inter_idx = size();
      child_iter->value = std::move(first_chunk);
      child_iter->child_idx = inter_idx;
      nodes_.emplace_back(node{}).parent = root_idx;

      /* create the right son node */
      assert(not right_chunk.empty());
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

      if (left_chunk.empty())
      {
        ++nodes_[inter_idx].freq;
        return inter_idx;
      }

      ++nodes_[left_idx].freq;
      return left_idx;
    }

    assert(sit == str.end());

    return insert_node(child_iter->child_idx, new_start, end);
  }

  template <typename Command>
  std::size_t Trie<Command>::size() const
  {
    return std::size(nodes_);
  }

  template <typename Command>
  unsigned Trie<Command>::get_distinct_queries() const
  {
    return distinct_queries_;
  }

  template <typename Command>
  template <typename... Args>
  Trie<Command>::Trie(Args&&... args)
      : cmd_{&nodes_, std::forward<Args>(args)...}
  {
    nodes_.resize(1);

    LOG("sizeof(node) = %lu\n", sizeof(node));
    LOG("sizeof(edge) = %lu\n", sizeof(edge));
  }

  template <typename Command>
  Trie<Command>::Trie()
      : cmd_{&nodes_}
  {
    nodes_.resize(1);

    LOG("sizeof(node) = %lu\n", sizeof(node));
    LOG("sizeof(edge) = %lu\n", sizeof(edge));
  }

  template <typename Command>
  void Trie<Command>::emplace(std::string_view str)
  {
    if (str.empty())
      return;

    BENCH_START(bench_insert, "node insertion");
    auto node_idx = insert_node(0, str.begin(), str.end());

    assert(nodes_.at(node_idx).freq > 0);

    cmd_(node_idx);
  }

  template <typename Command>
  typename Command::result_t Trie<Command>::command_get_result() const
  {
    return cmd_.result_get();
  }
} // namespace trie
