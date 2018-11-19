#include <trie/chunk_memory.hh>
#include <trie/distinct.hh>
#include <trie/top_list.hh>
#include <trie/trie.hh>

namespace
{
  bool check_string_memory()
  {
    trie::ChunkMemory chunks{};

    std::string_view elt{};
    {
      std::string str = "this is a test";
      auto itr = str.begin() + str.find(" a ");
      elt = chunks.push(str.begin(), itr);
      chunks.push(itr, str.end());
    }

    return elt == "this is" && chunks.size() == 2;
  }

  bool check_word_insertion()
  {
    trie::Trie tree{};

    if (tree.size() != 1)
      return false;

    tree.emplace("this");
    if (tree.size() != 2)
      return false;

    tree.emplace("those");
    if (tree.size() != 4)
      return false;

    tree.emplace("there");
    if (tree.size() != 5)
      return false;

    tree.emplace("them");
    if (tree.size() != 7)
      return false;

    tree.emplace("test");
    if (tree.size() != 9)
      return false;

    tree.emplace("hello");
    if (tree.size() != 10)
      return false;

    return true;
  }

  bool check_distinct_queries()
  {
    trie::Trie<trie::Distinct> tree{};

    tree.emplace("hello world");
    tree.emplace("hello");
    tree.emplace("world");
    tree.emplace("hello world");
    tree.emplace("hello");
    tree.emplace("hello world");

    return tree.command_get_result() == 3;
  }

  bool check_top_queries()
  {
    trie::Trie<trie::TopList> tree{3u};

    tree.emplace("hello world");
    tree.emplace("hello world");
    tree.emplace("hello");
    tree.emplace("world");
    tree.emplace("test");
    tree.emplace("world");
    tree.emplace("hello world");
    tree.emplace("hello");
    tree.emplace("hello");
    tree.emplace("hello world");

    auto top = tree.command_get_result();

    if (top.size() != 3)
      return false;
    if (const auto& [str, freq] = top.at(0); str != "hello world" || freq != 4)
      return false;
    if (const auto& [str, freq] = top.at(1); str != "hello" || freq != 3)
      return false;
    if (const auto& [str, freq] = top.at(2); str != "world" || freq != 2)
      return false;

    return true;
  }
} // namespace

int main()
{
  if (check_string_memory() && check_word_insertion()
      && check_distinct_queries() && check_top_queries())
    return 0;

  return 1;
}
