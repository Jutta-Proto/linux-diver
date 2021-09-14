#include "BLEDevice.hpp"
#include "Obfuscator.hpp"
#include "gattlib.h"
#include "jutta_proto/JuttaConnection.hpp"
#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <ios>
#include <iostream>
#include <logger/Logger.hpp>
#include <vector>
#include <spdlog/spdlog.h>

//---------------------------------------------------------------------------
namespace jutta_bt_driver {
//---------------------------------------------------------------------------
BLEDevice::BLEDevice(std::string&& name, std::string&& addr, OnCharacteristicReadFunc onCharacteristicRead, OnConnectedFunc onConnected) : name(std::move(name)),
                                                                                                                                           addr(std::move(addr)),
                                                                                                                                           onCharacteristicRead(std::move(onCharacteristicRead)),
                                                                                                                                           onConnected(std::move(onConnected)) {}

const std::vector<uint8_t> BLEDevice::to_vec(const uint8_t* data, size_t len) {
    const uint8_t* dataBuf = static_cast<const uint8_t*>(data);
    std::vector<uint8_t> result;
    result.resize(len);
    for (size_t i = 0; i < len; i++) {
        // NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-pointer-arithmetic)
        result[i] = static_cast<uint8_t>(dataBuf[i]);
    }
    return result;
}

const std::vector<uint8_t> BLEDevice::to_vec(const void* data, size_t len) {
    const uint8_t* dataBuf = static_cast<const uint8_t*>(data);
    return to_vec(dataBuf, len);
}

const std::vector<uint8_t> BLEDevice::get_mam_data() {
    gattlib_advertisement_data_t* adData = nullptr;
    size_t adDataCount = 0;
    uint16_t manId = 0;
    uint8_t* manData = nullptr;
    size_t manDataCount = 0;
    if (gattlib_get_advertisement_data(connection, &adData, &adDataCount, &manId, &manData, &manDataCount) != GATTLIB_SUCCESS) {
        SPDLOG_ERROR("BLE device advertisement data analysis failed.");
        return std::vector<uint8_t>();
    }
    return to_vec(manData, manDataCount);
}

bool BLEDevice::connect() {
    assert(!connection);
    connection = gattlib_connect(nullptr, addr.c_str(), GATTLIB_CONNECTION_OPTIONS_LEGACY_DEFAULT);
    if (!connection) {
        return false;
    }

    if (gattlib_discover_primary(connection, &services, &serviceCount) != GATTLIB_SUCCESS) {
        SPDLOG_ERROR("BLE device GATT discovery failed.");
        if (gattlib_disconnect(connection)) {
            SPDLOG_ERROR("BLE device disconnect failed.");
        }
        return false;
    }
    SPDLOG_INFO("Discovered {} services.", serviceCount);
    onConnected();
    return true;
}

void BLEDevice::read_characteristics() {
    std::array<char, MAX_LEN_UUID_STR + 1> uuidStr{};
    for (int i = 0; i < serviceCount; i++) {
        // NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-pointer-arithmetic)
        gattlib_uuid_to_string(&services[i].uuid, uuidStr.data(), uuidStr.size());
        SPDLOG_DEBUG("Found service with UUID: {}", uuidStr.data());
    }

    int characteristics_count = 0;
    gattlib_characteristic_t* characteristics{nullptr};
    gattlib_discover_char(connection, &characteristics, &characteristics_count);
    for (int i = 0; i < characteristics_count; i++) {
        // NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-pointer-arithmetic)
        gattlib_uuid_to_string(&characteristics[i].uuid, uuidStr.data(), uuidStr.size());
        void* buffer = nullptr;
        size_t bufLen = 0;
        // NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (gattlib_read_char_by_uuid(connection, &(characteristics[i].uuid), &buffer, &bufLen)) {
            SPDLOG_WARN("Read from characteristic without data!");
        } else {
            // Convert to a vector:
            const std::vector<uint8_t> data = to_vec(buffer, bufLen);
            // NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-pointer-arithmetic)
            onCharacteristicRead(data, characteristics[i].uuid);
        }
        SPDLOG_DEBUG("Found characteristic with UUID: {}", uuidStr.data());
    }
}
//---------------------------------------------------------------------------
}  // namespace jutta_bt_driver
//---------------------------------------------------------------------------