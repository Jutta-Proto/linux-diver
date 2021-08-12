#include "BLEDevice.hpp"
#include "gattlib.h"
#include <array>
#include <cassert>
#include <logger/Logger.hpp>
#include <spdlog/spdlog.h>

//---------------------------------------------------------------------------
namespace jutta_bt_driver {
//---------------------------------------------------------------------------
BLEDevice::BLEDevice(std::string&& name, std::string&& addr) : name(std::move(name)), addr(std::move(addr)) {}

bool BLEDevice::connect() {
    assert(!connection);
    connection = gattlib_connect(nullptr, addr.c_str(), GATTLIB_CONNECTION_OPTIONS_LEGACY_DEFAULT);
    if (!connection) {
        return false;
    }

    if (gattlib_discover_primary(connection, &services, &serviceCount)) {
        SPDLOG_ERROR("BLE device GATT discovery failed.");
        if (gattlib_disconnect(connection)) {
            SPDLOG_ERROR("BLE device disconnect failed.");
        }
        return false;
    }

    std::array<char, MAX_LEN_UUID_STR + 1> uuidStr{};
    for (int i = 0; i < serviceCount; i++) {
        gattlib_uuid_to_string(&services[i].uuid, uuidStr.data(), uuidStr.size());

        printf("service[%d] start_handle:%02x end_handle:%02x uuid:%s\n", i,
               services[i].attr_handle_start, services[i].attr_handle_end,
               uuidStr.data());
    }

    return true;
}
//---------------------------------------------------------------------------
}  // namespace jutta_bt_driver
//---------------------------------------------------------------------------