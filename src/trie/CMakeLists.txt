add_library(liba_trie STATIC
  top_list.cc
  distinct.cc)

target_link_libraries(liba_trie
  liba_tools
  )

add_subdirectory(tests)
