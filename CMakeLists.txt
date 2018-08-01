cmake_minimum_required(VERSION 3.8)
project (chratos-exchange LANGUAGES CXX C)

include_directories("${CMAKE_SOURCE_DIR}/src")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

add_subdirectory(gtest)
include_directories("gtest/include")

file(GLOB source_files "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB wallet_files "${CMAKE_SOURCE_DIR}/src/wallet/*.cpp")
file(GLOB utility_files "${CMAKE_SOURCE_DIR}/src/utility/*.cpp")
file(GLOB external_files "${CMAKE_SOURCE_DIR}/src/math/external/*.c")
file(GLOB math_files "${CMAKE_SOURCE_DIR}/src/math/*")

file (GLOB wallet_test_files "${CMAKE_SOURCE_DIR}/test/wallet/*.cpp")

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_library(wallet
  ${wallet_files}
)

add_library(math
  ${math_files}
  ${external_files}
)

add_executable(chratexd
  ${source_files}
)

add_executable(chratex_test
  ${wallet_test_files}
)

target_link_libraries(chratexd wallet math)

target_link_libraries(chratex_test gtest_main wallet math)

target_compile_features(chratexd PUBLIC cxx_std_11)
