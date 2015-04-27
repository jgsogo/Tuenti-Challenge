
cmake_minimum_required (VERSION 2.8.11)


# Copy xz-embedded to
set(SRCS    xz-embedded/linux/include/linux/xz.h
            xz-embedded/linux/lib/xz/xz_crc32.c
            xz-embedded/linux/lib/xz/xz_dec_lzma2.c
            xz-embedded/linux/lib/xz/xz_dec_stream.c
            xz-embedded/linux/lib/xz/xz_lzma2.h
            xz-embedded/linux/lib/xz/xz_private.h
            xz-embedded/linux/lib/xz/xz_stream.h
            xz-embedded/userspace/xz_config.h
        )

# Executable
include_directories(xz-embedded/userspace/
                    xz-embedded/linux/include/linux/
                    xz-embedded/linux/lib/xz/
                    )
add_executable (Tuenti_Challenge_3 main.cpp ${SRCS} ${datafiles})
set_property(TARGET Tuenti_Challenge_3 PROPERTY CXX_STANDARD 11)

# Copy datafiles - and numbers.txt.xz
file(GLOB datafiles "*.sql")
file(COPY ${datafiles} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY numbers.txt.xz DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
