cmake_minimum_required(VERSION 3.8)
project (chratos-exchange LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

file(GLOB source_files "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB wallet_files "${CMAKE_SOURCE_DIR}/src/wallet/*.cpp")
file(GLOB utility_files "${CMAKE_SOURCE_DIR}/src/utility/*.cpp")
file(GLOB external_files "${CMAKE_SOURCE_DIR}/src/math/external/*.c")
file(GLOB math_files "${CMAKE_SOURCE_DIR}/src/math/*")

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(chratexd
  ${source_files}
  ${wallet_files}
  ${utility_files}
  ${external_files}
  ${math_files}
)

target_compile_features(chratexd PUBLIC cxx_std_11)
