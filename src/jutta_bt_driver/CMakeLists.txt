cmake_minimum_required(VERSION 3.16)

add_library(jutta_bt_driver STATIC JuttaBTDriver.hpp
                                   JuttaBTDriver.cpp
                                   BLEHelper.cpp
                                   BLEHelper.hpp
                                   BLEDevice.cpp
                                   BLEDevice.hpp
                                   Obfuscator.cpp
                                   Obfuscator.hpp
                                   BTCoffeeMaker.cpp
                                   BTCoffeeMaker.hpp
                                   Utils.cpp
                                   Utils.hpp)
target_link_libraries(jutta_bt_driver PRIVATE jutta_driver jutta_proto logger
                                      PUBLIC gattlib)

install(TARGETS jutta_bt_driver)