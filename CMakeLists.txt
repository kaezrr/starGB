cmake_minimum_required(VERSION 3.11)
project(starGB VERSION 1.1.4)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wno-unused-variable)

find_package(SDL2 REQUIRED)  
find_package(spdlog REQUIRED)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/)
configure_file(version.h.in ${SOURCE_DIR}/version.h @ONLY)

include_directories(${SOURCE_DIR})
include_directories(${SOURCE_DIR}/cpu)
include_directories(${SOURCE_DIR}/mbcs)
include_directories(${SOURCE_DIR}/utils)
include_directories(${SOURCE_DIR}/video)
include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=undefined -fsanitize=address)
    target_link_options(${PROJECT_NAME} BEFORE PRIVATE -fsanitize=undefined -fsanitize=address)
endif()