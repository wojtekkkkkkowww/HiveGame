add_executable(GameViewer GameViewer.cpp main.cpp)
target_link_libraries(GameViewer PRIVATE GUI global_compile_options)

target_include_directories(GameViewer PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/gui/include
)
