add_executable(
  basics
  basics.cpp
)

target_link_libraries(
  basics
  GTest::gtest_main
  GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(basics)

