enable_testing()

file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(
  test_GameLogic
  ${TEST_SOURCES}
)

target_link_libraries(
  test_GameLogic
  GTest::gtest_main
  GameEngine
)

target_include_directories(
  test_GameLogic PRIVATE
  ${CMAKE_SOURCE_DIR}/engine/include  
)

include(GoogleTest)
gtest_discover_tests(test_GameLogic)