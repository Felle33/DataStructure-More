cmake_minimum_required(VERSION 3.22.1)
project(treap)

# Include directories
include_directories(${PROJECT_SOURCE_DIR})

set(SOURCES
        main.c
        treap.c
)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})