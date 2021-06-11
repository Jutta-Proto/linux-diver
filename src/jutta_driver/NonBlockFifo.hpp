#pragma once

#include <cstddef>
#include <filesystem>

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

    [[nodiscard]] const std::filesystem::path& get_path() const;

    void writeNb(const char* buffer, size_t len) const;
    void writeNb(const std::string& buffer) const;

 private:
    void open_pipe();
    void close_pipe();
};
//---------------------------------------------------------------------------
}  // namespace jutta_driver
//---------------------------------------------------------------------------
