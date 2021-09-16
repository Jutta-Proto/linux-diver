#include "JuttaBTDriver.hpp"
#include "BLEHelper.hpp"
#include "BTCoffeeMaker.hpp"
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
#include <gattlib.h>
#include <spdlog/spdlog.h>

//---------------------------------------------------------------------------
namespace jutta_bt_driver {
//---------------------------------------------------------------------------
JuttaBTDriver::JuttaBTDriver() { create_file_structure(); }

JuttaBTDriver::~JuttaBTDriver() {
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

void JuttaBTDriver::run() {
    SPDLOG_INFO("Starting Jutta driver...");
    shouldRun = true;
    try {
        // TODO: Init jutta connection here
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Failed to initialize Jutta connection with: {}", e.what());
        shouldRun = false;
        return;
    }
    assert(!rxTxThread);
    rxTxThread = std::make_optional<std::thread>(&JuttaBTDriver::rx_tx_thread_run, this);
    SPDLOG_INFO("Jutta driver started.");
    modeFile->replace_contents("1\n");
    while (shouldRun) {
        // TODO: Request device type
        std::shared_ptr<std::string> result = std::make_shared<std::string>();
        if (result) {
            size_t pos = result->find("ty:");
            if (pos == 0) {
                *result = result->substr(3);
            }
            deviceFile->replace_contents(*result);
            statusFile->replace_contents("1\n");
        } else {
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

void JuttaBTDriver::stop() {
    SPDLOG_INFO("Stopping Jutta driver...");
    shouldRun = false;
}

void JuttaBTDriver::create_file_structure() {
    if (!std::filesystem::exists(baseDirPath)) {
        if (!std::filesystem::create_directory(baseDirPath)) {
            throw std::runtime_error("Failed to create directory: " + baseDirPath.native());
        }
    }
    if (!std::filesystem::is_directory(baseDirPath)) {
        throw std::runtime_error(baseDirPath.native() + " is not a directory!");
    }

    txFifo = std::make_unique<jutta_driver::NonBlockFifo>(baseDirPath / TX_FIFO_FILE_NAME, jutta_driver::NonBlockFifoMode::READING);
    rxFifo = std::make_unique<jutta_driver::NonBlockFifo>(baseDirPath / RX_FIFO_FILE_NAME, jutta_driver::NonBlockFifoMode::WRITING);

    statusFile = std::make_unique<jutta_driver::StatusFile>(baseDirPath / STATUS_FILE_NAME);
    statusFile->replace_contents("0");
    modeFile = std::make_unique<jutta_driver::StatusFile>(baseDirPath / MODE_FILE_NAME);
    modeFile->replace_contents("0");
    deviceFile = std::make_unique<jutta_driver::StatusFile>(baseDirPath / DEVICE_FILE_NAME);
}

void JuttaBTDriver::rx_tx_thread_run() const {
    SPDLOG_DEBUG("RX/TX Thread started.");

    while (shouldRun) {
        // Scan for the device:
        std::shared_ptr<BTCoffeeMaker> device{nullptr};
        while (shouldRun && !device) {
            device = scan_for_device("TT214H BlueFrog");
            if (!device) {
                std::this_thread::sleep_for(std::chrono::seconds{2});
            }
        }

        if (device && shouldRun) {
            // Connect to the device:
            if (device->connect()) {
                SPDLOG_INFO("Connected!");
                while (shouldRun && device->is_connected()) {
                    std::this_thread::sleep_for(std::chrono::seconds{1});
                }
            }
        }
    }
    SPDLOG_DEBUG("RX/TX Thread stopped.");
}
//---------------------------------------------------------------------------
}  // namespace jutta_bt_driver
//---------------------------------------------------------------------------
