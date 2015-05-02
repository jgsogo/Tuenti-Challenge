
cmake_minimum_required (VERSION 2.8.11)

file(GLOB datafiles "*.sql")

add_executable (11_Crazy_Tower main.cpp Room.hpp ${datafiles})
set_property(TARGET 11_Crazy_Tower PROPERTY CXX_STANDARD 11)

file(COPY ${datafiles} scenarios.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
