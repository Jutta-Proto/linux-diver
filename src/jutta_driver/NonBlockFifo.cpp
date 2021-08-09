#include "NonBlockFifo.hpp"
#include "logger/Logger.hpp"
#include <array>
#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <exception>
#include <filesystem>
#include <stdexcept>
#include <system_error>
#include <fcntl.h>
#include <spdlog/spdlog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//---------------------------------------------------------------------------
namespace jutta_driver {
//---------------------------------------------------------------------------
NonBlockFifo::NonBlockFifo(std::filesystem::path&& path, NonBlockFifoMode mode) : path(std::move(path)), mode(mode) {
    SPDLOG_DEBUG("Opening FIFO '{}' in mode {}.", this->path.c_str(), this->mode);
    open_pipe();
}

NonBlockFifo::~NonBlockFifo() {
    close_pipe();
}

const std::filesystem::path& NonBlockFifo::get_path() const {
    return path;
}

NonBlockFifoMode NonBlockFifo::get_mode() const {
    return mode;
}

void NonBlockFifo::open_pipe() {
    // Try to create a FIFO in case it does not exist and test it:
    if (std::filesystem::exists(path)) {
        if (!std::filesystem::is_fifo(path)) {
            throw std::runtime_error("Failed to create FIFO at " + path.native() + " - file exists and is no FIFO.");
        }
    } else {
        int result = mkfifo(path.c_str(), 0666);
        if (result != 0) {
            // NOLINTNEXTLINE (concurrency-mt-unsafe)
            std::string errorStr = strerror(errno);
            throw std::runtime_error("Failed to create FIFO at " + path.native() + " - " + errorStr);
        }
        // Check again if the FIFO now exists and works:
        open_pipe();
        return;
    }

    // Open the file descriptor:
    if (mode == NonBlockFifoMode::READING) {
        fd = open(path.c_str(), O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    } else {
        fd = open(path.c_str(), O_RDWR | O_NONBLOCK | O_CLOEXEC);
    }
    if (fd < 0) {
        // NOLINTNEXTLINE (concurrency-mt-unsafe)
        std::string errorStr = strerror(errno);
        throw std::runtime_error("Failed to open FIFO '" + path.native() + "' with: " + errorStr);
    }
    SPDLOG_DEBUG("FIFO '{}' in mode {} opened.", path.c_str(), mode);
}

void NonBlockFifo::close_pipe() {
    if (std::filesystem::exists(path)) {
        if (!std::filesystem::remove(path)) {
            SPDLOG_WARN("Failed to remove FIFO at: {}", path.c_str());
        }
    }
    SPDLOG_DEBUG("FIFO '{}' in mode {} closed.", path.c_str(), mode);
}

void NonBlockFifo::writeNb(const char* buffer, size_t len) const {
    assert(mode == NonBlockFifoMode::WRITING);
    write(fd, buffer, len);
}

void NonBlockFifo::writeNb(const std::string& buffer) const {
    writeNb(buffer.c_str(), buffer.size());
}

void NonBlockFifo::writeNb(const std::vector<uint8_t>& buffer) const {
    assert(mode == NonBlockFifoMode::WRITING);
    write(fd, buffer.data(), buffer.size());
}

size_t NonBlockFifo::readNb(std::vector<uint8_t>* buffer) {
    assert(mode == NonBlockFifoMode::READING);
    size_t count = 0;
    // Read chunks of max. 20 bytes per iteration:
    std::array<uint8_t, 20> tmpBuffer{};
    // Read the first chunk of data:
    ssize_t readCount = read(fd, tmpBuffer.data(), tmpBuffer.size());
    while (readCount > 0) {
        count += readCount;
        size_t oldSize = buffer->size();
        SPDLOG_DEBUG("Resizing. oldSize: {}, readCount: {}, count: {}", oldSize, readCount, count);
        buffer->resize(oldSize + readCount);
        std::memcpy(&((*buffer)[oldSize - 1]), tmpBuffer.data(), readCount);

        // Read the next chunk of data:
        readCount = read(fd, tmpBuffer.data(), tmpBuffer.size());
    }
    SPDLOG_INFO("readNB: {}", count);
    return count;
}
//---------------------------------------------------------------------------
}  // namespace jutta_driver
//---------------------------------------------------------------------------
