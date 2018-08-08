cmake_minimum_required(VERSION 3.8)
project (libbitcoin LANGUAGES CXX C)

include_directories("${CMAKE_SOURCE_DIR}/include")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

include_directories (${Boost_INCLUDE_DIR})

file(GLOB source_files "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB chain_files "${CMAKE_SOURCE_DIR}/src/chain/*.cpp")
file(GLOB config_files "${CMAKE_SOURCE_DIR}/src/config/*.cpp")
file(GLOB formats_files "${CMAKE_SOURCE_DIR}/src/formats/*.cpp")
file(GLOB log_files "${CMAKE_SOURCE_DIR}/src/log/*.cpp")
file(GLOB machine_files "${CMAKE_SOURCE_DIR}/src/machine/*.cpp")
file(GLOB math_files "${CMAKE_SOURCE_DIR}/src/math/*.cpp")
file(GLOB math_external_files "${CMAKE_SOURCE_DIR}/src/math/external/*.c")
file(GLOB message_files "${CMAKE_SOURCE_DIR}/src/message/*.cpp")
file(GLOB unicode_files "${CMAKE_SOURCE_DIR}/src/unicode/*.cpp")
file(GLOB utility_files "${CMAKE_SOURCE_DIR}/src/utility/*.cpp")
file(GLOB wallet_files "${CMAKE_SOURCE_DIR}/src/wallet/*.cpp")

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_library(libbitcoin
  ${source_files}
  ${chain_files}
  ${config_files}
  ${formats_files}
  ${log_files}
  ${machine_files}
  ${math_files}
  ${math_external_files}
  ${message_files}
  ${unicode_files}
  ${utility_files}
  ${wallet_files}
)

target_compile_features(libbitcoin PUBLIC cxx_std_11)

