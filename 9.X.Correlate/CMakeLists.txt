
cmake_minimum_required (VERSION 2.8.11)

file(GLOB datafiles "*.sql")

add_executable (9_X_Correlate main.cpp ${datafiles})
set_property(TARGET 9_X_Correlate PROPERTY CXX_STANDARD 11)

file(COPY ${datafiles} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
