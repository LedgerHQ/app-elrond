cmake_minimum_required(VERSION 3.10)
project(test_parser C)

include(CTest)

set(CMAKE_C_STANDARD 11)

# required for fuzzing
set(CMAKE_C_COMPILER /usr/bin/clang)

enable_testing()

include_directories(../src ../deps/uint256/)

add_library(elrond
  ../src/parseTx.c
  ../src/parseTx.h
  ../deps/uint256/uint256.c
  ../deps/uint256/uint256.h
)

add_executable(fuzz_tx
  fuzz_tx.c
)

add_definitions(-DFUZZING)

target_compile_options(fuzz_tx PRIVATE -Wall -fsanitize=fuzzer,address -g -ggdb2)
target_link_libraries(fuzz_tx PRIVATE elrond -fsanitize=fuzzer,address)
