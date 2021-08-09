#pragma once

#include <dbus/dbus.h>

//---------------------------------------------------------------------------
namespace jutta_bt_driver {
//---------------------------------------------------------------------------
/**
 * Based on: https://github.com/makercrew/dbus-sample
 **/
class BTDBus {
 private:
    DBusConnection* connection{nullptr};
    DBusError error{};

 public:
    BTDBus();
    BTDBus(BTDBus&&) = default;
    BTDBus(const BTDBus&) = default;
    BTDBus& operator=(BTDBus&&) = default;
    BTDBus& operator=(const BTDBus&) = default;
    ~BTDBus();

    bool init();
};
//---------------------------------------------------------------------------
}  // namespace jutta_bt_driver
//---------------------------------------------------------------------------