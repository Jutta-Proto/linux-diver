#pragma once

#include "gattlib.h"
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>
#include <bluetooth/sdp.h>

//---------------------------------------------------------------------------
namespace jutta_bt_driver {
//---------------------------------------------------------------------------
class BLEDevice {
 public:
    using OnCharacteristicReadFunc = std::function<void(const std::vector<uint8_t>&, const uuid_t&)>;
    using OnConnectedFunc = std::function<void()>;

 private:
    const std::string name;
    const std::string addr;

    OnCharacteristicReadFunc onCharacteristicRead;
    OnConnectedFunc onConnected;

    gatt_connection_t* connection{nullptr};
    int serviceCount{0};
    gattlib_primary_service_t* services{nullptr};

 public:
    BLEDevice(std::string&& name, std::string&& addr, OnCharacteristicReadFunc onCharacteristicRead, OnConnectedFunc onConnected);
    BLEDevice(BLEDevice&&) = default;
    BLEDevice(const BLEDevice&) = default;
    BLEDevice& operator=(BLEDevice&&) = delete;
    BLEDevice& operator=(const BLEDevice&) = delete;
    ~BLEDevice() = default;

    bool connect();
    const std::vector<uint8_t> get_mam_data();
    void read_characteristics();

 private:
    static const std::vector<uint8_t> to_vec(const void* data, size_t len);
    static const std::vector<uint8_t> to_vec(const uint8_t* data, size_t len);
};
//---------------------------------------------------------------------------
}  // namespace jutta_bt_driver
//---------------------------------------------------------------------------