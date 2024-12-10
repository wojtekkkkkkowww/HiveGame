add_executable(AiTester AiTester.cpp)

target_link_libraries(AiTester ai SQLite::SQLite3 GameEngine)
