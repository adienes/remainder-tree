cmake_minimum_required(VERSION 2.8.12)
project(cppremtree)

add_subdirectory(pybind11)

# pybind11_DIR = "."
# find_package(pybind11 REQUIRED)

pybind11_add_module(cppremtree src/cppremtree.cpp)

#set_target_properties(gmp PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
#target_link_libraries(cppremtree PRIVATE ntl gmp gmpxx)
