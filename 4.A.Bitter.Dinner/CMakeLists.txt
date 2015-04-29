
cmake_minimum_required (VERSION 2.8.11)

file(GLOB datafiles "*.sql")

include_directories(${Boost_INCLUDE_DIRS})
add_executable (4_A_Bitter_Dinner main.cpp ${datafiles})
set_property(TARGET 4_A_Bitter_Dinner PROPERTY CXX_STANDARD 11)

file(COPY  ${datafiles} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
