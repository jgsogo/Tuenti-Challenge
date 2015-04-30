
cmake_minimum_required (VERSION 2.8.11)

file(GLOB datafiles "*.sql")

add_executable (5_The_One_Treasure main.cpp World.hpp Island.hpp ${datafiles})
set_property(TARGET 5_The_One_Treasure PROPERTY CXX_STANDARD 11)

file(COPY ${datafiles} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
