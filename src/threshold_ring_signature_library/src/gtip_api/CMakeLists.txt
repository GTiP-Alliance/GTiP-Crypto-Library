cmake_minimum_required(VERSION 3.0.2)

file(GLOB_RECURSE SOURCES_CPP *.cpp)

include_directories(${CMAKE_BINARY_DIR}/ntl/include
					${GTEST_INCLUDE_DIR}
					${GFLAGS_INCLUDE_DIR}
					${GLOG_INCLUDE_DIR}
					${CMAKE_HOME_DIRECTORY}/src/)
					
link_directories(${CMAKE_BINARY_DIR}/ntl/lib ${CMAKE_BINARY_DIR}/gmp/lib)

add_library(gtip_api SHARED ${SOURCES_CPP})
add_dependencies(gtip_api ntl gmp glog gtip)

target_link_libraries(gtip_api libntl.so libgmp.so gtip)
set_target_properties(gtip_api PROPERTIES VERSION 1 SOVERSION 1.0)
set_target_properties(gtip_api PROPERTIES LINK_FLAGS "-Wl,--default-symver")