cmake_minimum_required(VERSION 2.8.12)
project(cppremtree)

add_subdirectory(pybind11)
#find_package(pybind11 REQUIRED)


pybind11_add_module(cppremtree src/cppremtree.cpp)
