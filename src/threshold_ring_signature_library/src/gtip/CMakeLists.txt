cmake_minimum_required(VERSION 3.0.2)

file(GLOB_RECURSE SOURCES_CPP *.cpp)

include_directories(${CMAKE_BINARY_DIR}/libntl/include
					${GTEST_INCLUDE_DIR}
					${GFLAGS_INCLUDE_DIR}
					${GLOG_INCLUDE_DIR}
					${CMAKE_BINARY_DIR}/libsodium/include
					${CMAKE_HOME_DIRECTORY}/src/)
					
add_library(gtip SHARED ${SOURCES_CPP})
add_dependencies(gtip ntl glog gflags sodium)

target_link_libraries(gtip glog gflags ntl gmp sodium)
set_target_properties(gtip PROPERTIES VERSION 1 SOVERSION 1.0)

install(TARGETS gtip LIBRARY DESTINATION lib)
