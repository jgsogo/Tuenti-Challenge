
cmake_minimum_required (VERSION 2.8.11)

add_executable (1_The_Buffer main.cpp)
set_property(TARGET 1_The_Buffer PROPERTY CXX_STANDARD 11)

file(COPY testInput.sql submitInput.sql DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
