cmake_minimum_required(VERSION 3.16)

add_library(jutta_driver STATIC NonBlockFifo.hpp
                                NonBlockFifo.cpp
                                StatusFile.cpp
                                StatusFile.hpp)
target_link_libraries(jutta_driver PRIVATE logger stdc++fs)

install(TARGETS jutta_driver)