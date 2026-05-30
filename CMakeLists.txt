cmake_minimum_required(VERSION 3.15)

project(lsm-kv
  VERSION 0.1
  DESCRIPTION "Educational LSM-tree based key-value store"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES
  src/main.cpp
  src/db/db.cpp
  src/db/memtable.cpp
  src/db/sstable/sstable.cpp
  src/db/sstable/sstable_builder.cpp
  src/db/sstable/sstable_block_iterator.cpp
  src/db/sstable/sstable_iterator.cpp
  src/services/config_service.cpp
)

add_executable(lsm_kv ${SOURCES})

target_include_directories(lsm_kv PRIVATE ${PROJECT_SOURCE_DIR}/include)

if(MSVC)
  target_compile_options(lsm_kv PRIVATE /W4)
else()
  target_compile_options(lsm_kv PRIVATE -Wall -Wextra -Wpedantic)
endif()