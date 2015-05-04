
cmake_minimum_required (VERSION 2.8.11)

file(GLOB datafiles "*.sql")

include_directories(${Boost_INCLUDE_DIRS})
add_executable (11_Crazy_Tower main.cpp Room.hpp Scenario.hpp Hero.hpp ModularArithmetic.hpp ${datafiles})
set_property(TARGET 11_Crazy_Tower PROPERTY CXX_STANDARD 11)
target_link_libraries(11_Crazy_Tower ${Boost_LIBRARIES})

file(COPY ${datafiles} scenarios.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
