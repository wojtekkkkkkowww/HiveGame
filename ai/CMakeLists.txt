add_library(ai
    AiAlgorithm.hpp
    AiAlgorithm.cpp
    RandomAi.hpp
    RandomAi.cpp
    AlphaBethaSearch.hpp
    AlphaBethaSearch.cpp
)

target_include_directories(ai PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ai PUBLIC GameEngine)
target_link_libraries(ai PUBLIC global_compile_options)
#target_compile_options(ai PRIVATE -O3)