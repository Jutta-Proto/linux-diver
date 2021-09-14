#include "BTCoffeeMaker.hpp"
#include "Obfuscator.hpp"
#include "logger/Logger.hpp"
#include <spdlog/spdlog.h>

//---------------------------------------------------------------------------
namespace jutta_bt_driver {
//---------------------------------------------------------------------------
BTCoffeeMaker::BTCoffeeMaker(std::string&& name, std::string&& addr) : bleDevice(
                                                                           std::move(name),
                                                                           std::move(addr),
                                                                           [this](const std::vector<uint8_t>& data, const uuid_t& uuid) { this->on_characteristic_read(data, uuid); },
                                                                           [this]() { this->on_connected(); }) {}

std::string BTCoffeeMaker::parse_version(const std::vector<uint8_t>& data, size_t from, size_t to) {
    std::string result;
    for (size_t i = from; i <= to; i++) {
        if (data[i]) {
            result += static_cast<char>(data[i]);
        }
    }
    return result;
}

void BTCoffeeMaker::parse_about_data(const std::vector<uint8_t>& data) {
    blueFrogVersion = parse_version(data, 27, 34);
    coffeeMachineVersion = parse_version(data, 35, 50);
    SPDLOG_DEBUG("Found about data. BlueFrog Version: {} Coffee Machine Version: {}", blueFrogVersion, coffeeMachineVersion);
}

void BTCoffeeMaker::parse_machine_status(const std::vector<uint8_t>& data, uint8_t key) {
    std::vector<std::uint8_t> actData = deobfuscate(data, key);

    std::vector<uint8_t> alertVec;
    alertVec.resize(19);
    for (size_t i = 1; i < actData.size(); i++) {
        uint8_t b = actData[i];
        uint8_t b2 = ((b & 0xF0) >> 4) | ((b & 0xF) << 4);
        uint8_t b3 = ((b2 & 0xCC) >> 2) | ((b2 & 0x33) << 2);
        alertVec[i - 1] = ((b3 & 0xAA) >> 1) | ((b3 & 0x55) << 1);
    }
}

void BTCoffeeMaker::analyze_man_data() {
    parse_man_data(bleDevice.get_mam_data());
}

void BTCoffeeMaker::parse_man_data(const std::vector<uint8_t>& data) {
    key = data[0];
    bfMajVer = data[1];
    bfMinVer = data[2];
    articleNumber = to_uint16_t_little_endian(data, 4);
    machineNumber = to_uint16_t_little_endian(data, 6);
    serialNumber = to_uint16_t_little_endian(data, 8);
    machineProdDate = to_ymd(data, 10);
    machineProdDateUCHI = to_ymd(data, 12);
    unusedSecond = data[14];
    statusBits = data[15];
}

uint16_t BTCoffeeMaker::to_uint16_t_little_endian(const std::vector<uint8_t>& data, size_t offset) {
    return (static_cast<uint16_t>(data[offset + 1]) << 8) | static_cast<uint16_t>(data[offset]);
}

std::chrono::year_month_day BTCoffeeMaker::to_ymd(const std::vector<uint8_t>& data, size_t offset) {
    uint16_t date = to_uint16_t_little_endian(data, offset);
    return std::chrono::year(((date & 65024) >> 9) + 1990) / ((date & 480) >> 5) / (date & 31);
}

void BTCoffeeMaker::on_characteristic_read(const std::vector<uint8_t>& data, const uuid_t& uuid) {
    std::array<char, MAX_LEN_UUID_STR + 1> uuidStr{};
    gattlib_uuid_to_string(&uuid, uuidStr.data(), uuidStr.size());

    // About UUID:
    if (std::strcmp(uuidStr.data(), "5a401531-ab2e-2548-c435-08c300000710") == 0) {
        parse_about_data(data);
    }
    // Machine status:
    else if (std::strcmp(uuidStr.data(), "5a401524-ab2e-2548-c435-08c300000710") == 0) {
        parse_machine_status(data, key);
    } else {
        // TODO print
    }
}

void BTCoffeeMaker::on_connected() {
    bleDevice.read_characteristics();
}

bool BTCoffeeMaker::connect() {
    return bleDevice.connect();
}
//---------------------------------------------------------------------------
}  // namespace jutta_bt_driver
//---------------------------------------------------------------------------