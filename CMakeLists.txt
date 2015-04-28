project(symbol-lookup CXX)
cmake_minimum_required(VERSION 2.6.2)

set(CMAKE_SUPPRESS_REGENERATION TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -std=c++0x -O3 -fno-optimize-sibling-calls -pthread")

# Default include path.
include_directories(src)

if ($ENV{BACKTRACE_EMPTY})
    add_definitions(-DBACKTRACE_EMPTY)
endif()

####################
# Libraries
####################

add_library(utils
    src/utils/scoped_tp.cpp
    src/utils/scoped_tp.h
    src/utils/time.h
    src/utils/tp.cpp
    src/utils/tp.h
    src/utils/use_cpu.cpp
    src/utils/use_cpu.h
    src/utils/utils.cpp
    src/utils/utils.h
    )

#########################
# Executables
#########################

add_executable(tk-getpid src/benchmark/getpid.cpp)
target_link_libraries(tk-getpid utils pthread)

add_executable(tk-ust src/benchmark/ust.cpp src/benchmark/ust-tp.cpp)
target_link_libraries(tk-ust utils pthread dl lttng-ust)

add_executable(tk-mongo src/benchmark/mongo.cpp)
target_link_libraries(tk-mongo
    mongoclient boost_thread boost_system boost_regex boost_random
    utils dl lttng-ust)

add_executable(tk-libunwind src/benchmark/libunwind.cpp)
target_link_libraries(tk-libunwind
    unwind unwind-x86_64 utils)
