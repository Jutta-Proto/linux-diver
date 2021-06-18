#pragma once

#include "NonBlockFifo.hpp"
#include <filesystem>
#include <jutta_proto/JuttaConnection.hpp>
#include <memory>
#include <streambuf>
#include <string_view>

//---------------------------------------------------------------------------
namespace jutta_driver {
//---------------------------------------------------------------------------
constexpr std::string_view BASE_DIR_PATH = "/tmp/coffee_maker";
constexpr std::string_view TX_FIFO_FILE_NAME = "tx";
constexpr std::string_view RX_FIFO_FILE_NAME = "rx";

class JuttaDriver {
 private:
    const std::filesystem::path baseDirPath = BASE_DIR_PATH;

    std::unique_ptr<NonBlockFifo> txFifo{nullptr};
    std::unique_ptr<NonBlockFifo> rxFifo{nullptr};

    jutta_proto::JuttaConnection connection;
    bool shouldRun{false};

 public:
    explicit JuttaDriver(std::string&& device);
    JuttaDriver(JuttaDriver&&) = delete;
    JuttaDriver(const JuttaDriver&) = delete;
    JuttaDriver& operator=(JuttaDriver&&) = delete;
    JuttaDriver& operator=(const JuttaDriver&) = delete;
    ~JuttaDriver();

    void create_file_structure();

    void run();
    void stop();

 private:
    void register_sig_handler();
    void sig_handler(int signal);
};
//---------------------------------------------------------------------------
}  // namespace jutta_driver
//---------------------------------------------------------------------------
