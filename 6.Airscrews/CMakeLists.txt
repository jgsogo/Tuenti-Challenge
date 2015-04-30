
cmake_minimum_required (VERSION 2.8.11)

# Executable
add_executable (6_Airscrews main.cpp ${SRCS} ${datafiles})
set_property(TARGET 6_Airscrews PROPERTY CXX_STANDARD 11)

# Copy datafiles
file(GLOB datafiles "*.sql")
file(COPY ${datafiles} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sheet.data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
