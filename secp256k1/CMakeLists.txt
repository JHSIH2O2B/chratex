cmake_minimum_required(VERSION 3.8)
project (secp256k1 LANGUAGES C)

include_directories("${secp256k1_SOURCE_DIR}/include")

set(CMAKE_C_STANDARD 11)

include_directories (${Boost_INCLUDE_DIR})

file(GLOB source_files "${secp256k1_SOURCE_DIR}/src/*.c")

add_library(secp256k1
  ${source_files}
)

