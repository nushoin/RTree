set (CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")

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

add_executable(
  largeData
  largeData.cpp
)
target_link_libraries(
  largeData
  GTest::gtest_main
  GTest::gmock_main
)

add_executable(
  treeList
  treeList.cpp
)
target_link_libraries(
  treeList
  GTest::gtest_main
  GTest::gmock_main
)

add_executable(
  nearestNeighbour
  nearestNeighbour.cpp
)
target_link_libraries(
  nearestNeighbour
  GTest::gtest_main
  GTest::gmock_main
)

add_executable(
  highDim
  highDim.cpp
)
target_link_libraries(
  highDim
  GTest::gtest_main
  GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(basics)
gtest_discover_tests(memory)
gtest_discover_tests(badData)
gtest_discover_tests(largeData)
gtest_discover_tests(treeList)
gtest_discover_tests(nearestNeighbour)
gtest_discover_tests(highDim)

