
cmake_minimum_required (VERSION 2.8.11)

file(GLOB datafiles "*.sql")

add_executable (8_Alchemy_Pot main.cpp ${datafiles})
set_property(TARGET 8_Alchemy_Pot PROPERTY CXX_STANDARD 11)

file(COPY ${datafiles} book.data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
