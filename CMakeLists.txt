
cmake_minimum_required (VERSION 2.8.11)
project (Tuenti_Challenge)

# In case we need boost
set(BOOST_ROOT ${BOOST_ROOT} CACHE PATH "Path to boost on the system" )
find_package(Boost COMPONENTS thread date_time system chrono REQUIRED)
set( Boost_INCLUDE_DIRS ${BOOST_ROOT} CACHE INTERNAL "Internal")
set( Boost_LIBRARY_DIR ${BOOST_ROOT}/stage/lib CACHE INTERNAL "Internal")
set(Boost_USE_STATIC_LIBS ON CACHE INTERNAL "Internal")
set(Boost_USE_MULTITHREADED ON CACHE INTERNAL "Internal")

#add_subdirectory("1.The.Buffer")
#add_subdirectory("2.Almost.prime")
#add_subdirectory("3.Favourite.primes")
#add_subdirectory("4.A.Bitter.Dinner")
#add_subdirectory("5.The.One.Treasure")
#add_subdirectory("6.Airscrews")
#add_subdirectory("7.The.Perfect.Larry.Matching.Algorithm")
#add_subdirectory("8.Alchemy.Pot")
#add_subdirectory("9.X.Correlate")
add_subdirectory("11.Crazy.Tower")


