
cmake_minimum_required (VERSION 2.8.11)

file(GLOB datafiles "*.sql")

add_executable (Tuenti_Challenge_2 main.cpp ${datafiles})

file(COPY ${datafiles} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
