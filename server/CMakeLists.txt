add_executable(Server server.cpp)

target_link_libraries(Server PRIVATE sfml-network sfml-system global_compile_options)
