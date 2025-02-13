cmake_minimum_required(VERSION 3.0)

set (PROJECT_NAME "InterviewKit")
set (PROJECT_FILES main.c IPv4.h)

project(${PROJECT_NAME} LANGUAGES C)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wpedantic -Wextra")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb -Og")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")

add_library(IPv4 SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(IPv4 PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/IPv4.so"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}"
)

include_directories(SYSTEM ${CMAKE_SOURCE_DIR} pytest_c_testrunner/test/)
add_executable(${PROJECT_NAME} ${PROJECT_FILES})
target_link_libraries(${PROJECT_NAME} IPv4)


