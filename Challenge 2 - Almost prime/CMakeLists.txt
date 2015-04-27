
cmake_minimum_required (VERSION 2.8.11)

add_executable (Tuenti_Challenge_2 main.cpp)

file(GLOB datafiles "*.sql")
file(COPY ${datafiles} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
