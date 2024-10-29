add_library(ai
    AiAlgorithm.hpp
    RandomAi.hpp
    RandomAi.cpp
)

target_include_directories(ai PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ai PUBLIC GameEngine)