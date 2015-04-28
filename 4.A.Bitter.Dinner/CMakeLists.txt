
cmake_minimum_required (VERSION 2.8.11)

# Need Boost for multipreccission integers
set(BOOST_ROOT ${BOOST_ROOT} CACHE PATH "Path to boost on the system" )
find_package(Boost REQUIRED)
set( Boost_INCLUDE_DIRS ${BOOST_ROOT} CACHE INTERNAL "Internal")
set( Boost_LIBRARY_DIR ${BOOST_ROOT}/stage/lib CACHE INTERNAL "Internal")

file(GLOB datafiles "*.sql")

add_executable (4_A_Bitter_Dinner main.cpp ${datafiles})
set_property(TARGET 4_A_Bitter_Dinner PROPERTY CXX_STANDARD 11)

file(COPY  ${datafiles} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
