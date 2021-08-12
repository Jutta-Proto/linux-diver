#pragma once

#include "jutta_driver/NonBlockFifo.hpp"
#include "jutta_driver/StatusFile.hpp"
#include <filesystem>
#include <memory>
#include <optional>
#include <streambuf>
#include <string_view>
#include <thread>

//---------------------------------------------------------------------------
namespace jutta_bt_driver {
//---------------------------------------------------------------------------
constexpr std::string_view BASE_DIR_PATH = "/tmp/coffee_maker";
constexpr std::string_view TX_FIFO_FILE_NAME = "tx";
constexpr std::string_view RX_FIFO_FILE_NAME = "rx";
constexpr std::string_view STATUS_FILE_NAME = "status";
constexpr std::string_view MODE_FILE_NAME = "mode";
constexpr std::string_view DEVICE_FILE_NAME = "device";

class JuttaBTDriver {
 private:
    const std::filesystem::path baseDirPath = BASE_DIR_PATH;

    std::optional<std::thread> rxTxThread{std::nullopt};
    std::unique_ptr<jutta_driver::NonBlockFifo> txFifo{nullptr};
    std::unique_ptr<jutta_driver::NonBlockFifo> rxFifo{nullptr};

    std::unique_ptr<jutta_driver::StatusFile> statusFile{nullptr};
    std::unique_ptr<jutta_driver::StatusFile> modeFile{nullptr};
    std::unique_ptr<jutta_driver::StatusFile> deviceFile{nullptr};

    bool shouldRun{false};

 public:
    explicit JuttaBTDriver();
    JuttaBTDriver(JuttaBTDriver&&) = delete;
    JuttaBTDriver(const JuttaBTDriver&) = delete;
    JuttaBTDriver& operator=(JuttaBTDriver&&) = delete;
    JuttaBTDriver& operator=(const JuttaBTDriver&) = delete;
    ~JuttaBTDriver();

    void create_file_structure();

    void run();
    void stop();

 private:
    void rx_tx_thread_run() const;
};
//---------------------------------------------------------------------------
}  // namespace jutta_bt_driver
//---------------------------------------------------------------------------
