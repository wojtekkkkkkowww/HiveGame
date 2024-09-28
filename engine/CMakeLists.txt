file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "include/*.hpp")

add_library(GameEngine ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_include_directories(GameEngine PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)