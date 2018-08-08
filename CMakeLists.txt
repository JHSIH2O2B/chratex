cmake_minimum_required(VERSION 3.8)
project (chratos-exchange LANGUAGES CXX C)

include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories("gtest/include")
include_directories("libbitcoin/include")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

find_package (Boost
  REQUIRED COMPONENTS 
  filesystem system thread program_options
)
include_directories (${Boost_INCLUDE_DIR})

add_subdirectory(gtest)
add_subdirectory(libbitcoin)

add_library (lmdb
	lmdb/libraries/liblmdb/lmdb.h
	lmdb/libraries/liblmdb/mdb.c
	lmdb/libraries/liblmdb/midl.c)

if (WIN32)
	target_link_libraries(lmdb ntdll)
endif()

file(GLOB source_files "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB utility_files "${CMAKE_SOURCE_DIR}/src/utility/*.cpp")

file(GLOB test_files "${CMAKE_SOURCE_DIR}/test/*.cpp")

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_library(libchratex
  ${utility_files}
)

add_executable(chratexd
  ${source_files}
)

add_executable(chratex_test
  ${test_files}
)

target_link_libraries(chratexd
  libbitcoin
  libchratex
  lmdb
  ${Boost_LIBRARIES}
)

target_link_libraries(chratex_test 
  gtest_main
  ${Boost_LIBRARIES}
)

target_compile_features(chratexd PUBLIC cxx_std_11)
target_compile_features(chratex_test PUBLIC cxx_std_11)
