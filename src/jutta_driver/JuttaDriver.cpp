#include "JuttaDriver.hpp"
#include "jutta_driver/NonBlockFifo.hpp"
#include "logger/Logger.hpp"
#include <exception>
#include <filesystem>
#include <memory>
#include <stdexcept>

//---------------------------------------------------------------------------
namespace jutta_driver {
//---------------------------------------------------------------------------
JuttaDriver::JuttaDriver(std::string&& device) : connection(std::move(device)) {
    create_file_structure();
}

void JuttaDriver::run() {
}

void JuttaDriver::create_file_structure() {
    static std::filesystem::path baseDirPath = BASE_DIR_PATH;
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
