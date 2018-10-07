cmake_minimum_required(VERSION 3.0.2)
project(qredo VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fPIC")

find_package(Threads REQUIRED)

include(ExternalProject)

#gmp
ExternalProject_Add(libgmp
	URL https://gmplib.org/download/gmp/gmp-6.1.0.tar.xz
	DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/
	CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/libgmp-prefix/src/libgmp/configure --prefix=${CMAKE_CURRENT_BINARY_DIR}/libgmp CXXFLAGS=-fPIC
	BUILD_COMMAND make install
	)

add_library(gmp SHARED IMPORTED)
set(GMP_LIBRARY_PATH ${CMAKE_CURRENT_BINARY_DIR}/libgmp/lib)
set(GMP_LIBRARY ${GMP_LIBRARY_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}gmp${CMAKE_SHARED_LIBRARY_SUFFIX})
set_target_properties(gmp PROPERTIES IMPORTED_LOCATION ${GMP_LIBRARY})
install(DIRECTORY ${GMP_LIBRARY_PATH}/ DESTINATION lib FILES_MATCHING PATTERN "${CMAKE_SHARED_LIBRARY_PREFIX}*")

#ntl
ExternalProject_Add(libntl
	DEPENDS libgmp 
	URL https://www.shoup.net/ntl/ntl-11.3.0.tar.gz
	DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/
	BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR}/libntl-prefix/src/libntl/src/ ./configure SHARED=on DEF_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/libntl GMP_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/libgmp CXXFLAGS=-fPIC
	BUILD_COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR}/libntl-prefix/src/libntl/src/ make
	INSTALL_COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR}/libntl-prefix/src/libntl/src/ make install
	)

add_library(ntl SHARED IMPORTED)
add_dependencies(ntl libntl)
set(NTL_LIBRARY_PATH ${CMAKE_CURRENT_BINARY_DIR}/libntl/lib)
set(NTL_LIBRARY ${NTL_LIBRARY_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}ntl${CMAKE_SHARED_LIBRARY_SUFFIX})
set_target_properties(ntl PROPERTIES IMPORTED_LOCATION ${NTL_LIBRARY})
install(DIRECTORY ${NTL_LIBRARY_PATH}/ DESTINATION lib FILES_MATCHING PATTERN "${CMAKE_SHARED_LIBRARY_PREFIX}*")

# gtest
ExternalProject_Add(googletest
	CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/gtest"
	CMAKE_ARGS "-DBUILD_SHARED_LIBS=True"
	URL https://github.com/google/googletest/archive/release-1.8.0.zip
	)

set(GTEST_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/gtest/include)
set(GTEST_LIBRARY_PATH ${CMAKE_CURRENT_BINARY_DIR}/gtest/lib)
set(GTEST_LIBRARY ${GTEST_LIBRARY_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}gtest${CMAKE_SHARED_LIBRARY_SUFFIX})
add_library(gtest SHARED IMPORTED)
set_target_properties(gtest PROPERTIES IMPORTED_LOCATION ${GTEST_LIBRARY})
add_dependencies(gtest googletest)
install(DIRECTORY ${GTEST_LIBRARY_PATH}/ DESTINATION lib FILES_MATCHING PATTERN "${CMAKE_SHARED_LIBRARY_PREFIX}*")

# gflags
ExternalProject_Add(googleflags
	CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/gflags"
	CMAKE_ARGS "-DBUILD_SHARED_LIBS=True"
	URL https://github.com/gflags/gflags/archive/v2.2.1.zip
	)

set(GFLAGS_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/gflags/include)
set(GFLAGS_LIBRARY_PATH ${CMAKE_CURRENT_BINARY_DIR}/gflags/lib)
set(GFLAGS_LIBRARY ${GFLAGS_LIBRARY_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}gflags${CMAKE_SHARED_LIBRARY_SUFFIX})
add_library(gflags SHARED IMPORTED)
set_target_properties(gflags PROPERTIES IMPORTED_LOCATION ${GFLAGS_LIBRARY})
add_dependencies(gflags googleflags)
install(DIRECTORY ${GFLAGS_LIBRARY_PATH}/ DESTINATION lib FILES_MATCHING PATTERN "${CMAKE_SHARED_LIBRARY_PREFIX}*")

# glog
ExternalProject_Add(googlelog
	DEPENDS googleflags
	CMAKE_ARGS "-Dgflags_DIR=${CMAKE_CURRENT_BINARY_DIR}/gflags"
	CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/glog"
	CMAKE_ARGS "-DBUILD_SHARED_LIBS=True"
	URL https://github.com/google/glog/archive/v0.3.5.zip
	)

set(GLOG_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/glog/include)
set(GLOG_LIBRARY_PATH ${CMAKE_CURRENT_BINARY_DIR}/glog/lib)
set(GLOG_LIBRARY ${GLOG_LIBRARY_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}glog${CMAKE_SHARED_LIBRARY_SUFFIX})
add_library(glog SHARED IMPORTED)
set_target_properties(glog PROPERTIES IMPORTED_LOCATION ${GLOG_LIBRARY})
add_dependencies(glog googlelog)
install(DIRECTORY ${GLOG_LIBRARY_PATH}/ DESTINATION lib FILES_MATCHING PATTERN "${CMAKE_SHARED_LIBRARY_PREFIX}*")

#libsodium
ExternalProject_Add(libsodium
	URL https://github.com/jedisct1/libsodium/releases/download/1.0.16/libsodium-1.0.16.tar.gz
	DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}
	CONFIGURE_COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR}/libsodium-prefix/src/libsodium/ ./autogen.sh
	BUILD_COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR}/libsodium-prefix/src/libsodium/ ./configure --prefix=${CMAKE_CURRENT_BINARY_DIR}/libsodium
	INSTALL_COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR}/libsodium-prefix/src/libsodium/ make install
	)

add_library(sodium SHARED IMPORTED)
set(LIBSODIUM_PATH ${CMAKE_CURRENT_BINARY_DIR}/libsodium/lib)
set(LIBSODIUM ${LIBSODIUM_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}sodium${CMAKE_SHARED_LIBRARY_SUFFIX})
set_target_properties(sodium PROPERTIES IMPORTED_LOCATION ${LIBSODIUM})
install(DIRECTORY ${LIBSODIUM_PATH}/ DESTINATION lib FILES_MATCHING PATTERN "${CMAKE_SHARED_LIBRARY_PREFIX}*")

include(subdirs.cmake)

