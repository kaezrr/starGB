cmake_minimum_required(VERSION 3.10)
project(starGB VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 REQUIRED)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/)
set(PROJECT_VERSION ${PROJECT_VERSION})
set(PROJECT_NAME ${PROJECT_NAME})

configure_file(version.h.in ${SOURCE_DIR}/version.h @ONLY)
include_directories(${SOURCE_DIR})
include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})
link_directories(${SOURCE_DIR})

file(GLOB SOURCE_FILES ${SOURCE_DIR}/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
