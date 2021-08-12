#pragma once

#include "gattlib.h"
#include <string>

//---------------------------------------------------------------------------
namespace jutta_bt_driver {
//---------------------------------------------------------------------------
class BLEDevice {
 private:
    const std::string name;
    const std::string addr;

    gatt_connection_t* connection{nullptr};
    int serviceCount{0};
    gattlib_primary_service_t* services{nullptr};
    gattlib_characteristic_t* characteristics{nullptr};

 public:
    BLEDevice(std::string&& name, std::string&& addr);
    BLEDevice(BLEDevice&&) = default;
    BLEDevice(const BLEDevice&) = default;
    BLEDevice& operator=(BLEDevice&&) = delete;
    BLEDevice& operator=(const BLEDevice&) = delete;
    ~BLEDevice() = default;

    bool connect();
};
//---------------------------------------------------------------------------
}  // namespace jutta_bt_driver
//---------------------------------------------------------------------------