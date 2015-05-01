
cmake_minimum_required (VERSION 2.8.11)

file(GLOB datafiles "*.sql")

add_executable (7_The_Perfect_Larry_Matching_Algorithm main.cpp Girl.hpp ${datafiles})
set_property(TARGET 7_The_Perfect_Larry_Matching_Algorithm PROPERTY CXX_STANDARD 11)

file(COPY ${datafiles} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
