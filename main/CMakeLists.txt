add_executable(HiveGameApp main.cpp)

target_link_libraries(HiveGameApp GameEngine GUI sfml-graphics)

target_include_directories(HiveGameApp PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/gui/include
)

target_compile_definitions(HiveGameApp PUBLIC SOURCE_DIR="${CMAKE_SOURCE_DIR}")
