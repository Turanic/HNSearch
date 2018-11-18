add_executable(check_trie main.cc)
target_link_libraries(check_trie liba_trie)

add_test(NAME trie COMMAND check_trie)
