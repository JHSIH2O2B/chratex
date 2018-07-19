cmake_minimum_required(VERSION 3.8)
project (chratos-exchange LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

file(GLOB source_files "${CMAKE_SOURCE_DIR}/src/*.cc")

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(chratexd
  ${source_files}
)

target_compile_features(chratexd PUBLIC cxx_std_11)
