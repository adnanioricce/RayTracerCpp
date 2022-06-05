cmake_minimum_required(VERSION 3.2)

project(cmake-project-template)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(RAYTRACER_INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(RAYTRACER_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(RAYTRACER_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(src)
#add_subdirectory(test)


