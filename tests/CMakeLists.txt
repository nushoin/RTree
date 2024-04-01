add_executable(
  basics
  basics.cpp
)

add_executable(
  memory
  memory.cpp
)

target_link_libraries(
  basics
  GTest::gtest_main
  GTest::gmock_main
)

target_link_libraries(
  memory
  GTest::gtest_main
  GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(basics)
gtest_discover_tests(memory)

