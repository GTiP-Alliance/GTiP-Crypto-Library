cmake_minimum_required(VERSION 3.0.2)

file(GLOB_RECURSE SOURCES_CPP *.cpp)

include_directories(${CMAKE_BINARY_DIR}/ntl/include
					${GTEST_INCLUDE_DIR}
					${GFLAGS_INCLUDE_DIR}
					${GLOG_INCLUDE_DIR}
					${CMAKE_HOME_DIRECTORY}/src/)

link_directories(${CMAKE_BINARY_DIR}/ntl/lib ${CMAKE_BINARY_DIR}/gmp/lib)

add_executable(gtip_test ${SOURCES_CPP})
add_dependencies(gtip_test ntl gmp glog gtip)

target_link_libraries(gtip_test 
	libntl.so
	libgmp.so
	gtip
	gtest
	glog
	gflags)