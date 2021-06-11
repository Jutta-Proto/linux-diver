cmake_minimum_required(VERSION 3.16)

add_subdirectory(jutta_driver)

set(MAIN_EXECUTABLE ${PROJECT_NAME})

add_executable(${MAIN_EXECUTABLE} main.cpp)

target_link_libraries(${MAIN_EXECUTABLE} PRIVATE logger jutta_driver serial)
set_property(SOURCE main.cpp PROPERTY COMPILE_DEFINITIONS JUTTA_DRIVER_VERSION="${PROJECT_VERSION}" JUTTA_DRIVER_VERSION_NAME="${VERSION_NAME}")
