cmake_minimum_required(VERSION 3.8)
project (chratos-exchange LANGUAGES CXX C)

include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories("gtest/include")
include_directories("libbitcoin/include")
include_directories("lmdb/libraries")

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

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(WORKING_DIR
    "${CMAKE_SOURCE_DIR}/src/utility/platform/macos/working_dir.mm"
  )
  set(CMAKE_C_FLAGS "-x objective-c")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(WORKING_DIR
    "${CMAKE_SOURCE_DIR}/src/utility/platform/windows/working_dir.cpp"
  )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(WORKING_DIR 
    "${CMAKE_SOURCE_DIR}/src/utility/platform/posix/working_dir.cpp"
  )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
  set(WORKING_DIR 
    "${CMAKE_SOURCE_DIR}/src/utility/platform/posix/working_dir.cpp"
  )
else ()
	error ("Unknown platform: ${CMAKE_SYSTEM_NAME}")
endif ()

if (APPLE)
	set(PLATFORM_LINK_FLAGS "-framework Foundation -framework OpenCL")
elseif (WIN32)
	set(PLATFORM_LINK_FLAGS "")
else ()
	set(PLATFORM_LINK_FLAGS "-static-libgcc -static-libstdc++")
endif ()

SET (ACTIVE_NETWORK live_network CACHE STRING "Selects which network parameters are used")
set_property (CACHE ACTIVE_NETWORK PROPERTY STRINGS test_network beta_network live_network)

file(GLOB source_files "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB utility_files "${CMAKE_SOURCE_DIR}/src/utility/*.cpp")
file(GLOB net_files "${CMAKE_SOURCE_DIR}/src/net/*.cpp")
file(GLOB database_files "${CMAKE_SOURCE_DIR}/src/database/*.cpp")
file(GLOB test_files "${CMAKE_SOURCE_DIR}/test/*.cpp")

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_library(libchratex
  src/config.hpp
  ${net_files}
  ${utility_files}
  ${database_files}
  ${WORKING_DIR}
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
  ${PLATFORM_LINK_FLAGS}
)

target_link_libraries(chratex_test 
  gtest_main
  ${Boost_LIBRARIES}
)

target_compile_definitions(libbitcoin
	PUBLIC
		-DACTIVE_NETWORK=${ACTIVE_NETWORK}
)

target_compile_definitions(libchratex
	PUBLIC
		-DACTIVE_NETWORK=${ACTIVE_NETWORK}
)

target_compile_definitions(chratexd
	PUBLIC
		-DACTIVE_NETWORK=${ACTIVE_NETWORK}
)

target_compile_features(chratexd PUBLIC cxx_std_11)
target_compile_features(chratex_test PUBLIC cxx_std_11)
