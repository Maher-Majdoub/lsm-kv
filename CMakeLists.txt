cmake_minimum_required(VERSION 3.15)

project(lsm-kv
  VERSION 0.1
  DESCRIPTION "Educational LSM-tree based key-value store"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include headers
include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
  src/main.cpp
)

add_executable(lsm_kv ${SOURCES})

if (MSVC)
  target_compile_options(lsm_kv PRIVATE /W4)
else()
  target_compile_options(lsm_kv PRIVATE -Wall -Wextra -Wpedantic)
endif()