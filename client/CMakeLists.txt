add_executable(AiClient AiClient.cpp Client.cpp)

target_link_libraries(AiClient ai GameEngine sfml-network)

target_include_directories(AiClient PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/include
)

 
add_executable(HumanClient HumanClient.cpp Client.cpp)

target_link_libraries(HumanClient GameEngine GUI sfml-graphics sfml-network)

target_include_directories(HumanClient PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/gui/include
)

