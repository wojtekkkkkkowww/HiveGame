file(GLOB_RECURSE GUI_SOURCES "src/*.cpp")
file(GLOB_RECURSE GUI_HEADERS "include/*.hpp")

add_library(GUI ${GUI_SOURCES} ${GUI_HEADERS})

target_include_directories(GUI PUBLIC 
    ${CMAKE_SOURCE_DIR}/gui/include
    ${CMAKE_SOURCE_DIR}/engine/include 
)

target_link_libraries(GUI PRIVATE sfml-graphics)

target_compile_definitions(GUI PUBLIC SOURCE_DIR="${CMAKE_SOURCE_DIR}")
