#include "BTDBus.hpp"
#include "dbus/dbus-shared.h"
#include <logger/Logger.hpp>
#include <spdlog/spdlog.h>

//---------------------------------------------------------------------------
namespace jutta_bt_driver {
//---------------------------------------------------------------------------
BTDBus::BTDBus() {
    dbus_error_init(&error);
}

BTDBus::~BTDBus() {
    dbus_error_free(&error);
}

bool BTDBus::init() {
    connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
    if (!connection) {
        SPDLOG_ERROR("Failed to initialize DBus with: {} - {}", error.name, error.message);
        return false;
    }
    SPDLOG_INFO("DBus successfully initialized.");
    return true;
}
//---------------------------------------------------------------------------
}  // namespace jutta_bt_driver
//---------------------------------------------------------------------------