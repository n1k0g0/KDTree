cmake_minimum_required(VERSION 3.15)
project(KDTree)

set(CMAKE_CXX_STANDARD 14)

add_executable(KDTree prj.test/main.cpp prj.lab/kdtree.h kdtree.cpp)