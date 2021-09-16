#pragma once

#include "BLEDevice.hpp"
#include <chrono>
#include <optional>
#include <string>
#include <vector>
#include <bluetooth/sdp.h>

//---------------------------------------------------------------------------
namespace jutta_bt_driver {
//---------------------------------------------------------------------------
struct RelevantUUIDs {
    uuid_t DEFAULT_SERVICE_UUID{};
    uuid_t ABOUT_MACHINE_CHARACTERISTIC_UUID{};
    uuid_t MACHINE_STATUS_CHARACTERISTIC_UUID{};
    uuid_t BARISTA_MODE_CHARACTERISTIC_UUID{};
    uuid_t PRODUCT_PROGRESS_CHARACTERISTIC_UUID{};
    uuid_t P_MODE_CHARACTERISTIC_UUID{};
    uuid_t P_MODE_READ_CHARACTERISTIC_UUID{};
    uuid_t START_PRODUCT_CHARACTERISTIC_UUID{};
    uuid_t STATISTICS_COMMAND_CHARACTERISTIC_UUID{};
    uuid_t STATISTICS_DATA_CHARACTERISTIC_UUID{};
    uuid_t UPDATE_PRODUCT_CHARACTERISTIC_UUID{};

    uuid_t UART_SERVICE_UUID{};
    uuid_t UART_TX_CHARACTERISTIC_UUID{};
    uuid_t UART_RX_CHARACTERISTIC_UUID{};

    RelevantUUIDs() noexcept;

 private:
    static void to_uuid(const std::string& s, uuid_t* uuid);
} __attribute__((aligned(128)));

class BTCoffeeMaker {
 public:
    static const RelevantUUIDs RELEVANT_UUIDS;

 private:
    BLEDevice bleDevice;

    // Manufacturer advertisment data:
    uint8_t key{0};
    uint8_t bfMajVer{0};
    uint8_t bfMinVer{0};
    uint16_t articleNumber{0};
    uint16_t machineNumber{0};
    uint16_t serialNumber{0};
    std::chrono::year_month_day machineProdDate{};
    std::chrono::year_month_day machineProdDateUCHI{};
    uint8_t unusedSecond{0};
    uint8_t statusBits{0};

    // About data:
    std::string blueFrogVersion{};
    std::string coffeeMachineVersion{};

 public:
    explicit BTCoffeeMaker(std::string&& name, std::string&& addr);
    BTCoffeeMaker(BTCoffeeMaker&&) = default;
    BTCoffeeMaker(const BTCoffeeMaker&) = default;
    BTCoffeeMaker& operator=(BTCoffeeMaker&&) = delete;
    BTCoffeeMaker& operator=(const BTCoffeeMaker&) = delete;
    ~BTCoffeeMaker() = default;

    bool connect();
    bool is_connected();

 private:
    void analyze_man_data();
    void parse_man_data(const std::vector<uint8_t>& data);
    void parse_about_data(const std::vector<uint8_t>& data);
    static void parse_machine_status(const std::vector<uint8_t>& data, uint8_t key);
    static std::string parse_version(const std::vector<uint8_t>& data, size_t from, size_t to);

    static uint16_t to_uint16_t_little_endian(const std::vector<uint8_t>& data, size_t offset);
    static std::chrono::year_month_day to_ymd(const std::vector<uint8_t>& data, size_t offset);

    void on_characteristic_read(const std::vector<uint8_t>& data, const uuid_t& uuid);
    void on_connected();
    void on_disconnected();
};
//---------------------------------------------------------------------------
}  // namespace jutta_bt_driver
//---------------------------------------------------------------------------