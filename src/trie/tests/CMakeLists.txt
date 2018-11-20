if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_executable(check_trie main.cc)
  target_link_libraries(check_trie liba_trie)
endif()

add_test(NAME trie COMMAND check_trie)
