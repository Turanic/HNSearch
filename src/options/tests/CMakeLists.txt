if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_executable(check_options main.cc)
  target_link_libraries(check_options liba_options)
endif()

add_test(NAME options COMMAND check_options)
