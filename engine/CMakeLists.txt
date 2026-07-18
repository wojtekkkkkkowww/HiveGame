add_library(GameEngine
    src/ActionHandler.cpp
    src/ArticulationPoints.cpp
    src/Board.cpp
    src/FileOperations.cpp
    src/Game.cpp
    src/Player.cpp
)

target_include_directories(GameEngine PUBLIC ${CMAKE_SOURCE_DIR}/engine/include)

target_link_libraries(GameEngine PUBLIC global_compile_options)

if(HIVE_USE_STATIC_MAP)
    target_compile_definitions(GameEngine PUBLIC HIVE_USE_STATIC_MAP=1)
endif()
