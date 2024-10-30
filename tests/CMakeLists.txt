enable_testing()

file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(
  test_GameLogic
  ${TEST_SOURCES}
)

target_link_libraries(test_GameLogic PRIVATE
    GameEngine
    GUI
    GTest::GTest
    GTest::Main
    sfml-graphics
)

target_include_directories(
  test_GameLogic PRIVATE
  ${CMAKE_SOURCE_DIR}/engine/include  
  ${CMAKE_SOURCE_DIR}/gui/include

)
target_compile_definitions(test_GameLogic PUBLIC SOURCE_DIR="${CMAKE_SOURCE_DIR}")


include(GoogleTest)
gtest_discover_tests(test_GameLogic)
