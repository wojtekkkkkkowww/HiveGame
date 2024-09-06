enable_testing()

add_executable(
  test_GameEngine
  test_GameEngine.cpp
)

target_link_libraries(
  test_GameEngine
  GTest::gtest_main
  GameEngine
)

target_include_directories(
  test_GameEngine PRIVATE
  ${CMAKE_SOURCE_DIR}/engine  
)

include(GoogleTest)
gtest_discover_tests(test_GameEngine)
