add_executable(${CMAKE_PROJECT_NAME}
  main.cc)

target_link_libraries(${CMAKE_PROJECT_NAME}
  liba_options
  liba_tools
  liba_trie
  )
