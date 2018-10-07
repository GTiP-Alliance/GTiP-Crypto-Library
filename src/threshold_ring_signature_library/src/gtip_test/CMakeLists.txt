cmake_minimum_required(VERSION 3.0.2)

file(GLOB_RECURSE SOURCES_CPP *.cpp)

include_directories(${CMAKE_BINARY_DIR}/libntl/include
					${GTEST_INCLUDE_DIR}
					${GFLAGS_INCLUDE_DIR}
					${GLOG_INCLUDE_DIR}
					${CMAKE_BINARY_DIR}/libsodium/include
					${CMAKE_HOME_DIRECTORY}/src/)

add_executable(gtip_test ${SOURCES_CPP})
add_dependencies(gtip_test ntl glog gtip gflags)

target_link_libraries(gtip_test 
	glog
	gtest
	gflags
	ntl
	gmp
	gtip
	sodium)

install(TARGETS gtip_test RUNTIME DESTINATION bin)