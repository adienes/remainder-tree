cmake_minimum_required(VERSION 2.8.12)
project(cppremtree)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_subdirectory(pybind11)
pybind11_add_module(cppremtree SHARED src/cppremtree.cpp)

target_link_libraries(cppremtree PRIVATE ntl gmp gmpxx)


#set_target_properties(cppremtree PROPERTIES POSITION_INDEPENDENT_CODE ON)


#set_property(TARGET cppremtree PROPERTY POSITION_INDEPENDENT_CODE ON)

#set (EXTRA_OPTIONS "-fPIC")


#set_target_properties(cppremtree PROPERTIES COMPILE_FLAGS "-fPIC")



#add_library(ntl /usr/local/include/NTL)
