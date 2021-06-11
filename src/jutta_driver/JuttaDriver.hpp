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
constexpr std::string_view TX_FIFO_PATH = "/tmp/coffee_maker/tx";
constexpr std::string_view RX_FIFO_PATH = "/tmp/coffee_maker/rx";

class JuttaDriver {
 private:
    jutta_proto::JuttaConnection connection;

    std::unique_ptr<NonBlockFifo> txFifo{nullptr};
    std::unique_ptr<NonBlockFifo> rxFifo{nullptr};

 public:
    explicit JuttaDriver(std::string&& device);
    JuttaDriver(JuttaDriver&&) = default;
    JuttaDriver(const JuttaDriver&) = delete;
    JuttaDriver& operator=(JuttaDriver&&) = delete;
    JuttaDriver& operator=(const JuttaDriver&) = delete;
    ~JuttaDriver() = default;

    void create_file_structure();

    void run();
};
//---------------------------------------------------------------------------
}  // namespace jutta_driver
//---------------------------------------------------------------------------
