cmake_minimum_required(VERSION 3.15)
project(wire-dolphin)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed. Use a build/ directory.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  PcapPlusPlus
  GIT_REPOSITORY https://github.com/seladb/PcapPlusPlus.git
  GIT_TAG v25.05
)
FetchContent_MakeAvailable(PcapPlusPlus)

file(GLOB SRCS src/*.cpp)

add_executable(wire-shrimp ${SRCS})

target_include_directories(wire-shrimp PRIVATE src)

target_link_libraries(wire-shrimp
  PRIVATE
    Pcap++
    Packet++
    Common++
)

add_executable(tests tests/test_hello_world.cpp)
target_include_directories(tests PRIVATE src tests)
