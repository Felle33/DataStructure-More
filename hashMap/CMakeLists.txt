cmake_minimum_required(VERSION 3.22.1)
project(strHash)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
        main.c
        hashmap.c
)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})