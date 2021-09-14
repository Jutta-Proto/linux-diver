#pragma once

#include "BLEDevice.hpp"
#include <chrono>
#include <optional>
#include <string>
#include <vector>

//---------------------------------------------------------------------------
namespace jutta_bt_driver {
//---------------------------------------------------------------------------
class BTCoffeeMaker {
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
};
//---------------------------------------------------------------------------
}  // namespace jutta_bt_driver
//---------------------------------------------------------------------------