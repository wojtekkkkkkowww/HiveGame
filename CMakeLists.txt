cmake_minimum_required(VERSION 3.5)
project(HiveGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(GTest REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(SFML 2.5 COMPONENTS system network graphics REQUIRED)


add_subdirectory(engine)
add_subdirectory(gui)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(tests)
add_subdirectory(ai) 
add_subdirectory(ai_tests)
add_subdirectory(gameViewer)


add_library(global_compile_options INTERFACE)
target_compile_options(global_compile_options INTERFACE -Wall -Wextra -Wpedantic -Werror -std=c++17 -O3)


#add_compile_options(
#  -Wall 
#  -Wextra 
#  -Wpedantic 
#  -Werror 
#  -std=c++17
#  -O3
#)
