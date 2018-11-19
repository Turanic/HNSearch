#include "top_list.hh"
#include <algorithm>
#include <string>

namespace trie
{
  TopList::TopList(const std::vector<node>* nodes, unsigned top_limit)
      : nodes_{nodes}
      , top_limit_{top_limit}
  {
  }

  void TopList::operator()(index_t node_idx)
  {
    auto freq = nodes_->at(node_idx).freq;

    if (not top_queries_.empty() && node_idx == top_queries_.back())
      top_queries_.pop_back();

    if (top_queries_.size() >= top_limit_
        && nodes_->at(top_queries_.back()).freq >= freq)
      return;

    auto inf_it = std::find_if(top_queries_.begin(),
                               top_queries_.end(),
                               [this, freq, node_idx](auto top_idx) {
                                 return top_idx == node_idx
                                        || nodes_->at(top_idx).freq < freq;
                               });

    auto new_it = top_queries_.emplace(inf_it, node_idx);

    auto del_it = std::find(std::next(new_it), top_queries_.end(), node_idx);
    if (del_it != top_queries_.end())
      top_queries_.erase(del_it);
    else if (top_queries_.size() > top_limit_)
      top_queries_.pop_back();
  }

  TopList::result_t TopList::result_get() const
  {
    result_t res{};

    for (auto idx : top_queries_)
    {
      std::string word{};
      auto freq = nodes_->at(idx).freq;

      while (idx != 0)
      {
        const auto& node = nodes_->at(idx);
        const auto& parent = nodes_->at(node.parent);
        auto it =
          std::find_if(parent.children.begin(),
                       parent.children.end(),
                       [idx](const auto& e) { return e.child_idx == idx; });
        word = std::string{it->value} + word;
        idx = node.parent;
      }
      res.emplace_back(word, freq);
    }

    return res;
  }
} // namespace trie
