cmake_minimum_required(VERSION 3.16)

add_subdirectory(jutta_driver)
add_subdirectory(jutta_uart_driver)

# --------------------UART-Driver-------------------------------
set(UART_DRIVER_EXECUTABLE_NAME "uart_driver")
set(UART_DRIVER_EXECUTABLE_MAIN "main_uart.cpp")

add_executable(${UART_DRIVER_EXECUTABLE_NAME} ${UART_DRIVER_EXECUTABLE_MAIN})
target_link_libraries(${UART_DRIVER_EXECUTABLE_NAME} PRIVATE logger jutta_uart_driver serial)
set_property(SOURCE ${UART_DRIVER_EXECUTABLE_MAIN} PROPERTY COMPILE_DEFINITIONS JUTTA_DRIVER_VERSION="${PROJECT_VERSION}" JUTTA_DRIVER_VERSION_NAME="${VERSION_NAME}")
