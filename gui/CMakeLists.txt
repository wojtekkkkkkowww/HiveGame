add_library(GUI
    src/BaseInterface.cpp
    src/BoardDrawable.cpp
    src/Button.cpp
    src/GameController.cpp
    src/HexDrawable.cpp
)

target_include_directories(GUI PUBLIC 
    ${CMAKE_SOURCE_DIR}/gui/include
    ${CMAKE_SOURCE_DIR}/engine/include 
)

target_link_libraries(GUI PUBLIC GameEngine sfml-graphics global_compile_options)

target_compile_definitions(GUI PUBLIC SOURCE_DIR="${CMAKE_SOURCE_DIR}")
