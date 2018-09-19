cmake_minimum_required(VERSION 3.0.2)

file(GLOB_RECURSE SOURCES_CPP *.cpp)

include_directories(${CMAKE_BINARY_DIR}/libntl/include
					${GTEST_INCLUDE_DIR}
					${GFLAGS_INCLUDE_DIR}
					${GLOG_INCLUDE_DIR}
					${CMAKE_HOME_DIRECTORY}/src/)
					
add_library(gtip_api SHARED ${SOURCES_CPP})
add_dependencies(gtip_api libntl glog gtip)

target_link_libraries(gtip_api ntl gmp gtip)
set_target_properties(gtip_api PROPERTIES PUBLIC_HEADER "gtip_api.h")
set_target_properties(gtip_api PROPERTIES VERSION 1 SOVERSION 1.0)

install(TARGETS gtip_api 
	LIBRARY DESTINATION lib
	PUBLIC_HEADER DESTINATION include)