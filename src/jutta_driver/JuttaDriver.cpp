#include "JuttaDriver.hpp"
#include "jutta_driver/NonBlockFifo.hpp"
#include "jutta_driver/StatusFile.hpp"
#include "logger/Logger.hpp"
#include <chrono>
#include <cmath>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <memory>
#include <optional>
#include <stdexcept>
#include <thread>
#include <vector>
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

    statusFile = nullptr;
    modeFile = nullptr;
    deviceFile = nullptr;

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
    assert(!rxTxThread);
    rxTxThread = std::make_optional<std::thread>(&JuttaDriver::rx_tx_thread_run, this);
    SPDLOG_INFO("Jutta driver started.");
    modeFile->replace_contents("1\n");
    while (shouldRun) {
        std::shared_ptr<std::string> result = connection.write_decoded_with_response("TY:\r\n");
        if(result) {
            size_t pos = result->find("ty:");
            if (pos == 0) {
                *result = result->substr(3);
            }
            deviceFile->replace_contents(*result);
            statusFile->replace_contents("1\n");
        }   
        else {
            deviceFile->replace_contents("");
            statusFile->replace_contents("0\n");
        }
        std::this_thread::sleep_for(std::chrono::seconds{5});
    }
    statusFile->replace_contents("0\n");
    rxTxThread->join();
    rxTxThread = std::nullopt;
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

    statusFile = std::make_unique<StatusFile>(baseDirPath / STATUS_FILE_NAME);
    statusFile->replace_contents("0");
    modeFile = std::make_unique<StatusFile>(baseDirPath / MODE_FILE_NAME);
    modeFile->replace_contents("0");
    deviceFile = std::make_unique<StatusFile>(baseDirPath / DEVICE_FILE_NAME);
}

void JuttaDriver::rx_tx_thread_run() {
    SPDLOG_DEBUG("RX/TX Thread started.");
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
    SPDLOG_DEBUG("RX/TX Thread stopped.");
}
//---------------------------------------------------------------------------
}  // namespace jutta_driver
//---------------------------------------------------------------------------
