#include "JuttaDriver.hpp"
#include "jutta_driver/NonBlockFifo.hpp"
#include "logger/Logger.hpp"
#include <chrono>
#include <cmath>
#include <exception>
#include <filesystem>
#include <thread>
#include <vector>
#include <memory>
#include <stdexcept>
#include <bits/stdint-uintn.h>
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
    std::vector<uint8_t> readBuffer{};
    std::vector<uint8_t> writeBuffer{};
    bool wasAction = false;
    while (shouldRun) {
        wasAction = false;

        // Read:
        if(txFifo->readNb(&readBuffer) > 0) {
            if(!connection.write_decoded(readBuffer)) {
                SPDLOG_WARN("Something went wrong when writing to the coffee maker...");
            }
            readBuffer.clear();
            wasAction = true;
        }

        // Write:
        if(connection.read_decoded(writeBuffer)) {
            rxFifo->writeNb(writeBuffer);
            writeBuffer.clear();
            wasAction = true;
        }

        if(!wasAction) {
            std::this_thread::sleep_for(std::chrono::milliseconds{250});
        }
        else {
            std::this_thread::yield();
        }
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
