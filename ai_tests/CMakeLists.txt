add_executable(AiTester AiTester.cpp DataBase.cpp)

target_link_libraries(AiTester ai SQLite::SQLite3 GameEngine)
