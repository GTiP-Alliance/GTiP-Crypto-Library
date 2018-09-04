cmake_minimum_required(VERSION 3.0.2)

set(SRC qredo_ring.cpp)

include_directories(${CMAKE_BINARY_DIR}/ntl/include)
link_directories(${CMAKE_BINARY_DIR}/ntl/lib ${CMAKE_BINARY_DIR}/gmp/lib)

add_library(qredo_ring SHARED ${SRC})

target_link_libraries(qredo_ring libntl.a libgmp.a)
set_target_properties(qredo_ring PROPERTIES VERSION 1 SOVERSION 1.0)
set_target_properties(qredo_ring PROPERTIES LINK_FLAGS "-Wl,--default-symver")