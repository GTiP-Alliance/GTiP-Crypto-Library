cmake_minimum_required(VERSION 3.0.2)
project(GTiP VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Threads REQUIRED)

include(ExternalProject)

ExternalProject_Add(gmp
	URL https://gmplib.org/download/gmp/gmp-6.1.0.tar.xz
	DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/
	CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/gmp-prefix/src/gmp/configure --prefix=${CMAKE_CURRENT_BINARY_DIR}/gmp
	BUILD_COMMAND make install
	)

ExternalProject_Add(ntl
	DEPENDS gmp 
	URL https://www.shoup.net/ntl/ntl-11.2.1.tar.gz
	DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/
	BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR}/ntl-prefix/src/ntl/src/ ./configure DEF_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/ntl GMP_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/gmp
	BUILD_COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR}/ntl-prefix/src/ntl/src/ make
	INSTALL_COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR}/ntl-prefix/src/ntl/src/ make install
	)


include(subdirs.cmake)
