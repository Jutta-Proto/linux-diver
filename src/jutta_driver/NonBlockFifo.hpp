#pragma once

#include <cstddef>
#include <filesystem>
#include <vector>

//---------------------------------------------------------------------------
namespace jutta_driver {
//---------------------------------------------------------------------------
enum NonBlockFifoMode {
    READING,
    WRITING
};

class NonBlockFifo {
 private:
    std::filesystem::path path;
    NonBlockFifoMode mode;

    int fd{0};

 public:
    explicit NonBlockFifo(std::filesystem::path&& path, NonBlockFifoMode mode);
    NonBlockFifo(NonBlockFifo&&) = default;
    NonBlockFifo(const NonBlockFifo&) = default;
    NonBlockFifo& operator=(NonBlockFifo&&) = delete;
    NonBlockFifo& operator=(const NonBlockFifo&) = delete;
    ~NonBlockFifo();

    /**
     * Returns the path to the FIFO file.
     **/
    [[nodiscard]] const std::filesystem::path& get_path() const;
    [[nodiscard]] NonBlockFifoMode get_mode() const;

    /**
     * Writes the given buffer to the FIFO in non blocking mode.
     * This means, if there is no receiver, nobody will receive the data.
     **/
    void writeNb(const char* buffer, size_t len) const;
    /**
     * Writes the given buffer to the FIFO in non blocking mode.
     * This means, if there is no receiver, nobody will receive the data.
     **/
    void writeNb(const std::vector<uint8_t>& buffer) const;
    /**
     * Writes the given string to the FIFO in non blocking mode.
     * This means, if there is no receiver, nobody will receive the data.
     **/
    void writeNb(const std::string& buffer) const;
    /**
     * Reads everything available from the FIFO in a non blocking way and appends it to the given vector.
     * Returns the number of read bytes.
     **/
    size_t readNb(std::vector<uint8_t>* buffer);

 private:
    void open_pipe();
    void close_pipe();
};
//---------------------------------------------------------------------------
}  // namespace jutta_driver
//---------------------------------------------------------------------------
