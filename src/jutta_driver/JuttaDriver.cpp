#include "JuttaDriver.hpp"
#include "jutta_driver/NonBlockFifo.hpp"
#include "logger/Logger.hpp"
#include <chrono>
#include <cmath>
#include <exception>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <spdlog/spdlog.h>

//---------------------------------------------------------------------------
namespace jutta_driver {
//---------------------------------------------------------------------------
JuttaDriver::JuttaDriver(std::string&& device) : connection(std::move(device)) {
    create_file_structure();
}

JuttaDriver::~JuttaDriver() {
    txFifo = nullptr;
    rxFifo = nullptr;

    if (std::filesystem::exists(baseDirPath)) {
        if (!std::filesystem::remove(baseDirPath)) {
            SPDLOG_WARN("Failed to remove base directory at: {}", baseDirPath.c_str());
        }
    }
}

void JuttaDriver::run() {
    SPDLOG_INFO("Starting Jutta driver...");
    shouldRun = true;
    try {
        connection.init();
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Failed to initialize Jutta connection with: {}", e.what());
        shouldRun = false;
        return;
    }
    SPDLOG_INFO("Jutta driver stopped.");
    while (shouldRun) {
    }
    SPDLOG_INFO("Jutta driver stopped.");
}

void JuttaDriver::stop() {
    SPDLOG_INFO("Stopping Jutta driver...");
    shouldRun = false;
}

void JuttaDriver::create_file_structure() {
    if (!std::filesystem::exists(baseDirPath)) {
        if (!std::filesystem::create_directory(baseDirPath)) {
            throw std::runtime_error("Failed to create directory: " + baseDirPath.native());
        }
    }
    if (!std::filesystem::is_directory(baseDirPath)) {
        throw std::runtime_error(baseDirPath.native() + " is not a directory!");
    }

    txFifo = std::make_unique<NonBlockFifo>(baseDirPath / TX_FIFO_FILE_NAME, NonBlockFifoMode::READING);
    rxFifo = std::make_unique<NonBlockFifo>(baseDirPath / RX_FIFO_FILE_NAME, NonBlockFifoMode::WRITING);
}
//---------------------------------------------------------------------------
}  // namespace jutta_driver
//---------------------------------------------------------------------------
