#pragma once

#include "NonBlockFifo.hpp"
#include "StatusFile.hpp"
#include <filesystem>
#include <jutta_proto/JuttaConnection.hpp>
#include <memory>
#include <optional>
#include <streambuf>
#include <string_view>
#include <thread>
//---------------------------------------------------------------------------
namespace jutta_driver {
//---------------------------------------------------------------------------
constexpr std::string_view BASE_DIR_PATH = "/tmp/coffee_maker";
constexpr std::string_view TX_FIFO_FILE_NAME = "tx";
constexpr std::string_view RX_FIFO_FILE_NAME = "rx";
constexpr std::string_view STATUS_FILE_NAME = "status";
constexpr std::string_view MODE_FILE_NAME = "mode";
constexpr std::string_view DEVICE_FILE_NAME = "device";

class JuttaDriver {
 private:
    const std::filesystem::path baseDirPath = BASE_DIR_PATH;

    std::optional<std::thread> rxTxThread{std::nullopt};
    std::unique_ptr<NonBlockFifo> txFifo{nullptr};
    std::unique_ptr<NonBlockFifo> rxFifo{nullptr};

    std::unique_ptr<StatusFile> statusFile{nullptr};
    std::unique_ptr<StatusFile> modeFile{nullptr};
    std::unique_ptr<StatusFile> deviceFile{nullptr};

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
    void rx_tx_thread_run();
};
//---------------------------------------------------------------------------
}  // namespace jutta_driver
//---------------------------------------------------------------------------
