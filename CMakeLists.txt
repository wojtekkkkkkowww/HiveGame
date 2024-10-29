cmake_minimum_required(VERSION 3.5)
project(HiveGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    )
FetchContent_MakeAvailable(SFML)


enable_testing()


add_subdirectory(engine)
add_subdirectory(gui)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(tests)
add_subdirectory(ai) 

add_compile_options(
  -Wall 
  -Wextra 
  -Wpedantic 
  -Werror 
  -std=c++17
)
