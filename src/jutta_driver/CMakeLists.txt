cmake_minimum_required(VERSION 3.16)

add_library(jutta_driver SHARED JuttaDriver.hpp
                                JuttaDriver.cpp)
target_link_libraries(jutta_driver PRIVATE jutta_proto logger)

install(TARGETS jutta_driver)