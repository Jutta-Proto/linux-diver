#include "BLEHelper.hpp"
#include <logger/Logger.hpp>
#include <memory>
#include <gattlib.h>
#include <spdlog/spdlog.h>

//---------------------------------------------------------------------------
namespace jutta_bt_driver {
//---------------------------------------------------------------------------
void on_device_discovered(void* adapter, const char* addr, const char* name, void* userData) {
    ScanArgs* args = static_cast<ScanArgs*>(userData);
    args->m.lock();
    if (name && std::string{name} == args->name) {
        args->success = true;
        args->addr = addr;
        gattlib_adapter_scan_disable(adapter);
        SPDLOG_INFO("Coffee maker found!");
    }
    args->m.unlock();
    if (name) {
        SPDLOG_DEBUG("FOUND: {}", name);
    }
}

std::shared_ptr<BLEDevice> scan_for_device(std::string&& name) {
    SPDLOG_DEBUG("Scanning for devices...");
    void* adapter = nullptr;
    if (gattlib_adapter_open(nullptr, &adapter)) {
        SPDLOG_ERROR("Failed to open Bluetooth adapter.");
        return nullptr;
    }

    ScanArgs args{};
    args.name = std::move(name);

    size_t timeoutSeconds = 0;
    if (gattlib_adapter_scan_enable(adapter, &on_device_discovered, timeoutSeconds, &args)) {
        SPDLOG_ERROR("Bluetooth scan failed.");
        gattlib_adapter_close(adapter);
        return nullptr;
    }
    gattlib_adapter_close(adapter);
    SPDLOG_INFO("Scan stoped");
    if (args.success) {
        return std::make_shared<BLEDevice>(std::move(args.name), std::move(args.addr));
    }
    return nullptr;
}
//---------------------------------------------------------------------------
}  // namespace jutta_bt_driver
//---------------------------------------------------------------------------