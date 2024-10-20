add_executable(Server server.cpp)

target_link_libraries(Server GameEngine GUI sfml-network sfml-system)

target_include_directories(Server PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/gui/include
)

target_compile_definitions(Server PUBLIC SOURCE_DIR="${CMAKE_SOURCE_DIR}")
