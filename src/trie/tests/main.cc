#include <trie/chunk_memory.hh>
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
    trie::Trie<trie::TopList> tree{10u};

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
    trie::Trie<trie::TopList> tree{10u};

    tree.emplace("hello world");
    tree.emplace("hello");
    tree.emplace("world");
    tree.emplace("hello world");
    tree.emplace("hello");
    tree.emplace("hello world");

    return tree.get_distinct_queries() == 3;
  }
} // namespace

int main()
{
  if (check_string_memory() && check_word_insertion()
      && check_distinct_queries())
    return 0;

  return 1;
}
