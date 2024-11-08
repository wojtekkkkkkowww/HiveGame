file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "include/*.hpp")

add_library(GameEngine ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_include_directories(GameEngine PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)

target_link_libraries(GameEngine PUBLIC global_compile_options)
target_compile_options(GameEngine PRIVATE -O3)