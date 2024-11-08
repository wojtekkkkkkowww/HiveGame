add_executable(GameViewer GameViewer.cpp main.cpp)
target_link_libraries(GameViewer GameEngine GUI sfml-graphics)

target_include_directories(HumanClient PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/gui/include
)
target_link_libraries(GameViewer global_compile_options)
