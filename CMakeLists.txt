cmake_minimum_required (VERSION 3.9)
project (leveldb_sample)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/third_party/leveldb ${CMAKE_CURRENT_BINARY_DIR}/leveldb )

add_executable(sample sample.cpp)

target_link_libraries(sample leveldb)