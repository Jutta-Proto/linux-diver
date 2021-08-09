#include "jutta_bt_driver/JuttaBTDriver.hpp"
#include "logger/Logger.hpp"
#include <cassert>
#include <csignal>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <spdlog/spdlog.h>

// NOLINTNEXTLINE (cppcoreguidelines-avoid-non-const-global-variables)
std::unique_ptr<jutta_bt_driver::JuttaBTDriver> driver{nullptr};

void sig_handler(int signal) {
    if (signal == SIGTERM || signal == SIGINT) {
        assert(driver);
        driver->stop();
    }
}

void register_sig_handlers() {
    std::signal(SIGTERM, sig_handler);
    std::signal(SIGINT, sig_handler);
}

int main(int argc, char** /*argv*/) {
    logger::setup_logger(spdlog::level::debug);
    SPDLOG_INFO("Launching Version: {} {}", JUTTA_DRIVER_VERSION, JUTTA_DRIVER_VERSION_NAME);

    if (argc < 1) {
        SPDLOG_ERROR("Something went wrong... Argument count < 1: {}", argc);
        return -1;
    }

    try {
        driver = std::make_unique<jutta_bt_driver::JuttaBTDriver>();
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Failed to initialize driver with: {}", e.what());
        return -4;
    }

    register_sig_handlers();
    driver->run();
    // Set the driver to NULL, to prevent the logger from failing during destructor:
    driver = nullptr;
    return 0;
}
