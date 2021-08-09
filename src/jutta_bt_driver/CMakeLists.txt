cmake_minimum_required(VERSION 3.16)

add_library(jutta_bt_driver STATIC JuttaBTDriver.hpp
                                   JuttaBTDriver.cpp
                                   BTDBus.cpp
                                   BTDBus.hpp)
target_link_libraries(jutta_bt_driver PRIVATE jutta_driver jutta_proto logger ${DBUS_LIBRARIES})

install(TARGETS jutta_bt_driver)