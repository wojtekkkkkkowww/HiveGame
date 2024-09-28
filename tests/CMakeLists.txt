enable_testing()

file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(
  test_GameLogic
  ${TEST_SOURCES}
)

target_link_libraries(test_GameLogic PRIVATE
    GameEngine
    GUI
    gtest
    gtest_main
    sfml-graphics
    sfml-window
    sfml-system
)

target_include_directories(
  test_GameLogic PRIVATE
  ${CMAKE_SOURCE_DIR}/engine/include  
  ${CMAKE_SOURCE_DIR}/gui/include

)

include(GoogleTest)
gtest_discover_tests(test_GameLogic)


# tests/CMakeLists.txt



