file(GLOB_RECURSE GUI_SOURCES "src/*.cpp")
file(GLOB_RECURSE GUI_HEADERS "include/*.h")

add_library(GUI ${GUI_SOURCES} ${GUI_HEADERS})

target_include_directories(GUI PUBLIC ${CMAKE_SOURCE_DIR}/gui/include)
