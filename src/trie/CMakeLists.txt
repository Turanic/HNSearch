add_library(liba_trie STATIC
  trie.cc)

target_link_libraries(liba_trie
  liba_tools
  )
