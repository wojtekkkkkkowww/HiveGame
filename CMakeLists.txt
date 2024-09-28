cmake_minimum_required(VERSION 3.14)
project(HiveGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

set(SFML_DIR "/usr/lib64/cmake/SFML") 
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

enable_testing()


add_subdirectory(engine)
add_subdirectory(gui)
add_subdirectory(main)
add_subdirectory(tests)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)