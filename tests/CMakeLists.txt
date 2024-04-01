add_executable(
  basics
  basics.cpp
)
target_link_libraries(
  basics
  GTest::gtest_main
  GTest::gmock_main
)

add_executable(
  memory
  memory.cpp
)
target_link_libraries(
  memory
  GTest::gtest_main
  GTest::gmock_main
)

# copy bad data input file to build folder
configure_file(baddata.txt baddata.txt COPYONLY)
add_executable(
  badData
  badData.cpp
)
target_link_libraries(
  badData
  GTest::gtest_main
  GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(basics)
gtest_discover_tests(memory)
gtest_discover_tests(badData)

