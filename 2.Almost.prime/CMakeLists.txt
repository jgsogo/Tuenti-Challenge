
cmake_minimum_required (VERSION 2.8.11)

file(GLOB datafiles "*.sql")

add_executable (2_Almost_Prime main.cpp ${datafiles})
set_property(TARGET 2_Almost_Prime PROPERTY CXX_STANDARD 11)

file(COPY ${datafiles} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
