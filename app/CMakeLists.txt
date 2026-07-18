add_executable(HiveGame main.cpp)

target_include_directories(HiveGame PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/gui/include
    ${CMAKE_SOURCE_DIR}/ai
)

target_link_libraries(HiveGame PRIVATE GUI ai GameEngine sfml-graphics global_compile_options)
