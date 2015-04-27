
cmake_minimum_required (VERSION 2.8.11)

add_executable (Tuenti_Challenge_1 main.cpp)
file(COPY testInput.sql submitInput.sql DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
