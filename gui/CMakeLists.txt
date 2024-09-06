file(GLOB_RECURSE GUI_SOURCES "*.cpp")
file(GLOB_RECURSE GUI_HEADERS "*.h")

add_library(GUI ${GUI_SOURCES} ${GUI_HEADERS})
target_link_libraries(GUI sfml-graphics sfml-window sfml-system)
