cmake_minimum_required(VERSION 3.0.2)
project(qredo VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fPIC")

find_package(Threads REQUIRED)

include(ExternalProject)

ExternalProject_Add(gmp
	URL https://gmplib.org/download/gmp/gmp-6.1.0.tar.xz
	DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/
	CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/gmp-prefix/src/gmp/configure --prefix=${CMAKE_CURRENT_BINARY_DIR}/gmp CXXFLAGS=-fPIC
	BUILD_COMMAND make install
	)

ExternalProject_Add(ntl
	DEPENDS gmp 
	URL https://www.shoup.net/ntl/ntl-11.3.0.tar.gz
	DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/
	BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR}/ntl-prefix/src/ntl/src/ ./configure DEF_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/ntl GMP_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/gmp CXXFLAGS=-fPIC
	BUILD_COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR}/ntl-prefix/src/ntl/src/ make
	INSTALL_COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR}/ntl-prefix/src/ntl/src/ make install
	)

# gtest
ExternalProject_Add(googletest
	CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/gtest"
	URL https://github.com/google/googletest/archive/release-1.8.0.zip
	)

set(GTEST_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/gtest/include)
set(GTEST_LIBRARY_PATH ${CMAKE_CURRENT_BINARY_DIR}/gtest/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a)
set(GTEST_LIBRARY gtest)
add_library(${GTEST_LIBRARY} UNKNOWN IMPORTED)
set_property(TARGET ${GTEST_LIBRARY} PROPERTY IMPORTED_LOCATION
                ${GTEST_LIBRARY_PATH} )
add_dependencies(${GTEST_LIBRARY} googletest)

# gflags
ExternalProject_Add(googleflags
	CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/gflags"
	URL https://github.com/gflags/gflags/archive/v2.2.1.zip
	)

set(GOPPA_CODE_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/src/goppa_code)
set(GFLAGS_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/gflags/include)
set(GFLAGS_LIBRARY_PATH ${CMAKE_CURRENT_BINARY_DIR}/gflags/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gflags.a)
set(GFLAGS_LIBRARY gflags)

add_library(${GFLAGS_LIBRARY} UNKNOWN IMPORTED)
set_property(TARGET ${GFLAGS_LIBRARY} PROPERTY IMPORTED_LOCATION
                ${GFLAGS_LIBRARY_PATH} )
add_dependencies(${GFLAGS_LIBRARY} googleflags)

# glog
ExternalProject_Add(googlelog
	DEPENDS googleflags
	CMAKE_ARGS "-Dgflags_DIR=${CMAKE_CURRENT_BINARY_DIR}/gflags"
	CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/glog"
	URL https://github.com/google/glog/archive/v0.3.5.zip
	)

set(GLOG_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/glog/include)
set(GLOG_LIBRARY_PATH ${CMAKE_CURRENT_BINARY_DIR}/glog/lib/${CMAKE_FIND_LIBRARY_PREFIXES}glog.a)
set(GLOG_LIBRARY glog)
add_library(${GLOG_LIBRARY} UNKNOWN IMPORTED)
set_property(TARGET ${GLOG_LIBRARY} PROPERTY IMPORTED_LOCATION
                ${GLOG_LIBRARY_PATH} )
add_dependencies(${GLOG_LIBRARY} googlelog)

include(subdirs.cmake)

