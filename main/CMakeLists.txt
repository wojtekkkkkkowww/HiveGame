add_executable(HiveGameApp main.cpp)

target_link_libraries(HiveGameApp GameEngine GUI)

target_include_directories(HiveGameApp PRIVATE
    ${CMAKE_SOURCE_DIR}/engine
    ${CMAKE_SOURCE_DIR}/gui
)
