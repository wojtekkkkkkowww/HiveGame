cmake_minimum_required(VERSION 3.20)
project(HiveGame LANGUAGES CXX)

option(HIVE_USE_STATIC_MAP "Use fixed-capacity board storage" OFF)
option(HIVE_ENABLE_CLANG_TIDY "Run clang-tidy while compiling" OFF)

if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml@2")
    execute_process(COMMAND xcrun --show-sdk-path OUTPUT_VARIABLE CMAKE_OSX_SYSROOT OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(CMAKE_OSX_SYSROOT ${CMAKE_OSX_SYSROOT} CACHE PATH "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(global_compile_options INTERFACE)
if(MSVC)
    target_compile_options(global_compile_options INTERFACE /W4 /WX)
else()
    target_compile_options(global_compile_options INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        $<$<CONFIG:Release>:-O3>
    )
endif()

if(HIVE_ENABLE_CLANG_TIDY)
    find_program(HIVE_CLANG_TIDY_EXECUTABLE NAMES clang-tidy REQUIRED)
    set(HIVE_CLANG_TIDY_COMMAND
        "${HIVE_CLANG_TIDY_EXECUTABLE};--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy"
    )
endif()

function(hive_enable_clang_tidy target)
    if(HIVE_ENABLE_CLANG_TIDY)
        set_property(TARGET ${target} PROPERTY CXX_CLANG_TIDY "${HIVE_CLANG_TIDY_COMMAND}")
    endif()
endfunction()

find_package(GTest REQUIRED)
find_package(SFML COMPONENTS system network graphics REQUIRED)


add_subdirectory(engine)
add_subdirectory(gui)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(tests)
add_subdirectory(ai) 
add_subdirectory(ai_tests)
add_subdirectory(gameViewer)
add_subdirectory(app)
