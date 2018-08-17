cmake_minimum_required(VERSION 3.8)
project (chratos-exchange LANGUAGES CXX C)

include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories("gtest/include")
include_directories("libbitcoin/include")
include_directories("lmdb/libraries")
include_directories("phc-winner-argon2/include")
include_directories(".")

option(RAIBLOCKS_SIMD_OPTIMIZATIONS "Enable CPU-specific SIMD optimizations (SSE/AVX or NEON, e.g.)" OFF)

if (WIN32)
	add_definitions(-D_WIN32_WINNT=0x0600
					-DWINVER=0x0600
					-DWIN32_LEAN_AND_MEAN
					-DMINIUPNP_STATICLIB)
	set (BLAKE2_IMPLEMENTATION "blake2/blake2b.c")
else (WIN32)
	add_compile_options(-Werror=switch)
	if (RAIBLOCKS_ASAN)
		add_compile_options(-fsanitize=address,undefined)
		add_definitions(-DED25519_NO_INLINE_ASM)
	elseif (RAIBLOCKS_ASAN_INT)
		add_compile_options(-fsanitize=address,undefined,integer)
		add_definitions(-DED25519_NO_INLINE_ASM)
	elseif (RAIBLOCKS_TSAN)
		add_compile_options(-fsanitize=thread)
		add_definitions(-DED25519_NO_INLINE_ASM)
	endif()

	IF (CMAKE_SYSTEM_PROCESSOR MATCHES "^(i.86|x86(_64)?)$")
		if (RAIBLOCKS_SIMD_OPTIMIZATIONS)
			add_compile_options(-msse4)
		elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
			add_compile_options(-msse2)
		endif()

		set (BLAKE2_IMPLEMENTATION "blake2/blake2b.c")
		if (ENABLE_AVX2)
			add_compile_options(-mavx2 -mbmi -mbmi2)
			if (PERMUTE_WITH_GATHER)
				add_definitions(-DPERMUTE_WITH_GATHER)
			elseif (PERMUTE_WITH_SHUFFLES)
				add_definitions(-DPERMUTE_WITH_SHUFFLES)
			endif()
		endif()
	else()
		set (BLAKE2_IMPLEMENTATION "blake2/blake2b-ref.c")
	endif()
endif (WIN32)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

find_package (Boost 1.67.0
  REQUIRED COMPONENTS 
  filesystem system thread program_options random regex
)
include_directories (${Boost_INCLUDE_DIR})

add_subdirectory(gtest)
add_subdirectory(libbitcoin)
add_subdirectory(ed25519-donna)

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
file(GLOB lib_files "${CMAKE_SOURCE_DIR}/src/lib/*.cpp")
file(GLOB node_files "${CMAKE_SOURCE_DIR}/src/node/*.cpp")
file(GLOB wallet_files "${CMAKE_SOURCE_DIR}/src/wallet/*.cpp")

file(GLOB test_files "${CMAKE_SOURCE_DIR}/test/*.cpp")
file(GLOB wallet_test_files "${CMAKE_SOURCE_DIR}/test/wallet/*.cpp")

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set (BUILD_SHARED OFF CACHE BOOL "")
set (BUILD_TESTING OFF CACHE BOOL "")
set (USE_INTERMEDIATE_OBJECTS_TARGET OFF CACHE BOOL "")
if (CRYPTOPP_CUSTOM OR NOT RAIBLOCKS_SIMD_OPTIMIZATIONS)
	set (CRYPTOPP_LIBRARY cryptopp)
	add_library (cryptopp
		cryptopp/algparam.cpp
		cryptopp/asn.cpp
		cryptopp/basecode.cpp
		cryptopp/cpu.cpp
		cryptopp/cryptlib.cpp
		cryptopp/default.cpp
		cryptopp/des.cpp
		cryptopp/dessp.cpp
		cryptopp/dll.cpp
		cryptopp/ec2n.cpp
		cryptopp/ecp.cpp
		cryptopp/filters.cpp
		cryptopp/fips140.cpp
		cryptopp/gcm.cpp
		cryptopp/gf2n.cpp
		cryptopp/gfpcrypt.cpp
		cryptopp/hex.cpp
		cryptopp/hmac.cpp
		cryptopp/hrtimer.cpp
		cryptopp/integer.cpp
		cryptopp/iterhash.cpp
		cryptopp/misc.cpp
		cryptopp/modes.cpp
		cryptopp/mqueue.cpp
		cryptopp/nbtheory.cpp
		cryptopp/oaep.cpp
		cryptopp/osrng.cpp
		cryptopp/pubkey.cpp
		cryptopp/queue.cpp
		cryptopp/randpool.cpp
		cryptopp/rdtables.cpp
		cryptopp/rijndael.cpp
		cryptopp/rng.cpp
		cryptopp/sha.cpp
		cryptopp/simple.cpp
		cryptopp/winpipes.cpp)
else ()
	set (CRYPTOPP_LIBRARY cryptopp-static)
	add_subdirectory (cryptopp)
endif ()

if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
	set (ARGON_CORE phc-winner-argon2/src/opt.c)
else ()
	set (ARGON_CORE phc-winner-argon2/src/ref.c)
endif ()

add_library(argon2
	phc-winner-argon2/src/argon2.c
	phc-winner-argon2/include/argon2.h
	phc-winner-argon2/src/core.c
	phc-winner-argon2/src/thread.c
	phc-winner-argon2/src/encoding.c
	${ARGON_CORE}
)

target_include_directories(argon2 PUBLIC phc-winner-argon2/include)
target_include_directories(argon2 PUBLIC phc-winner-argon2/src)
target_include_directories(argon2 PUBLIC blake2)

add_library(blake2
	blake2/blake2-config.h
	blake2/blake2-impl.h
	blake2/blake2.h
	${BLAKE2_IMPLEMENTATION}
)

target_compile_definitions(blake2 PRIVATE -D__SSE2__)

add_library(libchratex
  src/config.hpp
  ${lib_files}
  ${net_files}
  ${utility_files}
  ${database_files}
  ${wallet_files}
  ${WORKING_DIR}
)

add_executable(chratexd
  ${source_files}
)

add_executable(chratex_test
  ${test_files}
  ${wallet_test_files}
)

target_link_libraries(chratexd
  libbitcoin 
  ed25519
  libchratex
  ${Boost_LIBRARIES}
  ${PLATFORM_LINK_FLAGS}
  ${CRYPTOPP_LIBRARY}
)

target_link_libraries(chratex_test 
  libbitcoin 
  gtest_main
  libchratex
  ${Boost_LIBRARIES}
  ${PLATFORM_LINK_FLAGS}
  ${CRYPTOPP_LIBRARY}
)

target_compile_definitions(libchratex
	PUBLIC
	  -DACTIVE_NETWORK=${ACTIVE_NETWORK}
)

target_compile_definitions(chratexd
	PUBLIC
		-DACTIVE_NETWORK=${ACTIVE_NETWORK}
)

target_compile_definitions(chratex_test
	PUBLIC
		-DACTIVE_NETWORK=${ACTIVE_NETWORK}
)

target_compile_features(chratexd PUBLIC cxx_std_11)
target_compile_features(chratex_test PUBLIC cxx_std_11)
