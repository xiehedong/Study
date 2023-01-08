cmake_minimum_required(VERSION 3.2)

project(demo)

set(ROOT_DIR ${CMAKE_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

aux_source_directory(src SRC_DIR)

set(PROJECT_MATH ${SRC_DIR})

add_executable(${PROJECT_NAME} ${PROJECT_MATH})