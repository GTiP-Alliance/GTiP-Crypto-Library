cmake_minimum_required(VERSION 3.0.2)
project(qredo VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fPIC")

find_package(Threads REQUIRED)

include(ExternalProject)

ExternalProject_Add(libgmp
	URL https://gmplib.org/download/gmp/gmp-6.1.0.tar.xz
	DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/
	CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/libgmp-prefix/src/libgmp/configure --prefix=${CMAKE_CURRENT_BINARY_DIR}/libgmp CXXFLAGS=-fPIC
	BUILD_COMMAND make install
	)

add_library(gmp SHARED IMPORTED)
set(GMP_LIBRARY_PATH ${CMAKE_CURRENT_BINARY_DIR}/libgmp/lib/${CMAKE_SHARED_LIBRARY_PREFIX}gmp${CMAKE_SHARED_LIBRARY_SUFFIX})
set_target_properties(gmp PROPERTIES IMPORTED_LOCATION ${GMP_LIBRARY_PATH})
install(FILES ${GMP_LIBRARY_PATH} DESTINATION lib)

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
set(NTL_LIBRARY_PATH ${CMAKE_CURRENT_BINARY_DIR}/libntl/lib/${CMAKE_SHARED_LIBRARY_PREFIX}ntl${CMAKE_SHARED_LIBRARY_SUFFIX})
set_target_properties(ntl PROPERTIES IMPORTED_LOCATION ${NTL_LIBRARY_PATH})
install(FILES ${NTL_LIBRARY_PATH} DESTINATION lib)

# gtest
ExternalProject_Add(googletest
	CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/gtest"
	CMAKE_ARGS "-DBUILD_SHARED_LIBS=True"
	URL https://github.com/google/googletest/archive/release-1.8.0.zip
	)

set(GTEST_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/gtest/include)
set(GTEST_LIBRARY_PATH ${CMAKE_CURRENT_BINARY_DIR}/gtest/lib/${CMAKE_SHARED_LIBRARY_PREFIX}gtest${CMAKE_SHARED_LIBRARY_SUFFIX})
set(GTEST_LIBRARY gtest)
add_library(${GTEST_LIBRARY} SHARED IMPORTED)
set_property(TARGET ${GTEST_LIBRARY} PROPERTY IMPORTED_LOCATION
                ${GTEST_LIBRARY_PATH} )
add_dependencies(${GTEST_LIBRARY} googletest)
install(FILES ${GTEST_LIBRARY_PATH} DESTINATION lib)

# gflags
ExternalProject_Add(googleflags
	CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/gflags"
	CMAKE_ARGS "-DBUILD_SHARED_LIBS=True"
	URL https://github.com/gflags/gflags/archive/v2.2.1.zip
	)

set(GOPPA_CODE_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/src/goppa_code)
set(GFLAGS_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/gflags/include)
set(GFLAGS_LIBRARY_PATH ${CMAKE_CURRENT_BINARY_DIR}/gflags/lib/${CMAKE_SHARED_LIBRARY_PREFIX}gflags${CMAKE_SHARED_LIBRARY_SUFFIX})
set(GFLAGS_LIBRARY gflags)

add_library(${GFLAGS_LIBRARY} SHARED IMPORTED)
set_property(TARGET ${GFLAGS_LIBRARY} PROPERTY IMPORTED_LOCATION
                ${GFLAGS_LIBRARY_PATH} )
add_dependencies(${GFLAGS_LIBRARY} googleflags)
install(FILES ${GFLAGS_LIBRARY_PATH} DESTINATION lib)

# glog
ExternalProject_Add(googlelog
	DEPENDS googleflags
	CMAKE_ARGS "-Dgflags_DIR=${CMAKE_CURRENT_BINARY_DIR}/gflags"
	CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/glog"
	CMAKE_ARGS "-DBUILD_SHARED_LIBS=True"
	URL https://github.com/google/glog/archive/v0.3.5.zip
	)

set(GLOG_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/glog/include)
set(GLOG_LIBRARY_PATH ${CMAKE_CURRENT_BINARY_DIR}/glog/lib/${CMAKE_SHARED_LIBRARY_PREFIX}glog${CMAKE_SHARED_LIBRARY_SUFFIX})
set(GLOG_LIBRARY glog)
add_library(${GLOG_LIBRARY} SHARED IMPORTED)
set_property(TARGET ${GLOG_LIBRARY} PROPERTY IMPORTED_LOCATION
                ${GLOG_LIBRARY_PATH} )
add_dependencies(${GLOG_LIBRARY} googlelog)
install(FILES ${GLOG_LIBRARY_PATH} DESTINATION lib)

include(subdirs.cmake)

