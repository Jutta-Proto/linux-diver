cmake_minimum_required(VERSION 3.16)

add_subdirectory(jutta_driver)
add_subdirectory(jutta_uart_driver)
add_subdirectory(jutta_bt_driver)

# --------------------UART-Driver-------------------------------
set(UART_DRIVER_EXECUTABLE_NAME "uart_driver")
set(UART_DRIVER_EXECUTABLE_MAIN "uart_main.cpp")

add_executable(${UART_DRIVER_EXECUTABLE_NAME} ${UART_DRIVER_EXECUTABLE_MAIN})
target_link_libraries(${UART_DRIVER_EXECUTABLE_NAME} PRIVATE logger jutta_uart_driver serial)
set_property(SOURCE ${UART_DRIVER_EXECUTABLE_MAIN} PROPERTY COMPILE_DEFINITIONS JUTTA_DRIVER_VERSION="${PROJECT_VERSION}" JUTTA_DRIVER_VERSION_NAME="${VERSION_NAME}")

# --------------------BT-Driver---------------------------------
set(BT_DRIVER_EXECUTABLE_NAME "bt_driver")
set(BT_DRIVER_EXECUTABLE_MAIN "bt_main.cpp")

add_executable(${BT_DRIVER_EXECUTABLE_NAME} ${BT_DRIVER_EXECUTABLE_MAIN})
target_link_libraries(${BT_DRIVER_EXECUTABLE_NAME} PRIVATE logger jutta_bt_driver)
set_property(SOURCE ${BT_DRIVER_EXECUTABLE_MAIN} PROPERTY COMPILE_DEFINITIONS JUTTA_DRIVER_VERSION="${PROJECT_VERSION}" JUTTA_DRIVER_VERSION_NAME="${VERSION_NAME}")
