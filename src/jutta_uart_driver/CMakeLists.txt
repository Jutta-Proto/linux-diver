cmake_minimum_required(VERSION 3.16)

add_library(jutta_uart_driver STATIC JuttaUartDriver.hpp
                                     JuttaUartDriver.cpp)
target_link_libraries(jutta_uart_driver PRIVATE jutta_driver jutta_proto logger)

install(TARGETS jutta_uart_driver)