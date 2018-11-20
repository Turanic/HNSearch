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
    while (start != end)
    {
      assert(root_idx < nodes_.size());

      auto& children = nodes_[root_idx].children;
      auto child_iter = std::find_if(children.begin(),
                                     children.end(),
                                     [start](const auto& elt) {
                                       assert(elt.value_ptr != nullptr);
                                       return *start == *elt.value_ptr;
                                     });

      /* First case: Completly new fragment */
      if (child_iter == children.end())
      {
        return create_new_child(root_idx, start, end);
      }
      assert(child_iter != children.end());

      auto str = child_iter->value_ptr;
      auto str_end = str + child_iter->value_len;
      auto [new_start, sit] = std::mismatch(start, end, str, str_end);

      /* Second case: Insert a new node between this one and the child */
      if (sit != str_end)
      {
        return break_edge(root_idx, &*child_iter, sit, new_start, end);
      }
      assert(sit == str_end);

      root_idx = child_iter->child_idx;
      start = new_start;
    }

    ++nodes_[root_idx].freq;
    return root_idx;
  }

  template <typename Command>
  template <typename InputIt>
  index_t
  Trie<Command>::create_new_child(index_t root_idx, InputIt start, InputIt end)
  {
    auto& children = nodes_[root_idx].children;
    auto new_idx = size();
    auto chunk = chunks_.push(start, end);
    children.emplace_back(chunk.data(), chunk.size(), new_idx);
    auto& new_node = nodes_.emplace_back(node{});
    new_node.parent = root_idx;
    ++new_node.freq;

    return new_idx;
  }

  template <typename Command>
  template <typename InputIt>
  index_t Trie<Command>::break_edge(index_t root_idx,
                                    edge* edge_ptr,
                                    InputIt break_loc,
                                    InputIt start,
                                    InputIt end)
  {
    auto off = std::distance(edge_ptr->value_ptr, break_loc);
    std::string_view first_chunk(edge_ptr->value_ptr, off);
    std::string_view right_chunk(edge_ptr->value_ptr + off,
                                 edge_ptr->value_len - off);
    std::string_view left_chunk(chunks_.push(start, end));
    auto old_child_idx = edge_ptr->child_idx;

    /* create the new intermediate node*/
    assert(not first_chunk.empty());
    auto inter_idx = size();
    edge_ptr->value_ptr = first_chunk.data();
    edge_ptr->value_len = first_chunk.size();
    edge_ptr->child_idx = inter_idx;
    nodes_.emplace_back(node{}).parent = root_idx;

    /* create the right son node */
    assert(not right_chunk.empty());
    nodes_[inter_idx].children.emplace_back(right_chunk.data(),
                                            right_chunk.size(),
                                            old_child_idx);
    nodes_[old_child_idx].parent = inter_idx;

    auto left_idx = size();
    if (not left_chunk.empty())
    {
      /* create the left son node */
      nodes_[inter_idx].children.emplace_back(left_chunk.data(),
                                              right_chunk.size(),
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

  template <typename Command>
  std::size_t Trie<Command>::size() const
  {
    return std::size(nodes_);
  }

  template <typename Command>
  template <typename... Args>
  Trie<Command>::Trie(Args&&... args)
      : cmd_{&nodes_, std::forward<Args>(args)...}
  {
    init();
  }

  template <typename Command>
  Trie<Command>::Trie()
      : cmd_{&nodes_}
  {
    init();
  }

  template <typename Command>
  void Trie<Command>::init()
  {
    LOG("sizeof(node) = %lu\n", sizeof(node));
    LOG("sizeof(edge) = %lu\n", sizeof(edge));
    nodes_.resize(1);
    // root node

    nodes_.reserve(1024 * 1024);
    // should be enought. If not realloc is not thaaat costly
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
