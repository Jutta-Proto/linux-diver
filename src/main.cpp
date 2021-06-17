#include "jutta_driver/JuttaDriver.hpp"
#include "logger/Logger.hpp"
#include <csignal>
#include <filesystem>
#include <iostream>
#include <cassert>
#include <memory>
#include <stdexcept>
#include <spdlog/spdlog.h>

// NOLINTNEXTLINE (cppcoreguidelines-avoid-non-const-global-variables)
std::unique_ptr<jutta_driver::JuttaDriver> driver{nullptr};

void print_help(const std::string& fileName) {
    std::cout << "Usage:\n";
    std::cout << "./" << fileName << " <path>\n";
    std::cout << "Example:\n";
    std::cout << "./" << fileName << " /dev/serial0\n";
}

void sig_handler(int signal) {
    if(signal == SIGTERM || signal == SIGINT) {
        assert(driver);
        driver->stop();
    }
}

void register_sig_handlers() {
    std::signal(SIGTERM, sig_handler);
    std::signal(SIGINT, sig_handler);
}

int main(int argc, char** argv) {
    logger::setup_logger(spdlog::level::debug);
    SPDLOG_INFO("Launching Version: {} {}", JUTTA_DRIVER_VERSION, JUTTA_DRIVER_VERSION_NAME);

    if (argc < 1) {
        SPDLOG_ERROR("Something went wrong... Argument count < 1: {}", argc);
        return -1;
    }
    // NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-pointer-arithmetic)
    std::string execName = std::filesystem::path(argv[0]).filename();

    if (argc != 2) {
        print_help(execName);
        return 0;
    }

    // NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-pointer-arithmetic)
    std::string path = argv[1];
    if (path == "--help") {
        print_help(execName);
        return 0;
    }

    std::filesystem::path serialPath = path;
    if (!std::filesystem::exists(serialPath)) {
        SPDLOG_ERROR("Path '{}' does not exist!", path.c_str());
        return -2;
    }

    if (!std::filesystem::is_character_file(serialPath)) {
        SPDLOG_ERROR("Path '{}' is not a valid UART (character) file!", path.c_str());
        return -3;
    }

    try {
        driver = std::make_unique<jutta_driver::JuttaDriver>(serialPath);
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Failed to initialize driver with: {}", e.what());
        return -4;
    }

    register_sig_handlers();
    driver->run();
    return 0;
}
