#pragma once

#include "BTCoffeeMaker.hpp"
#include <memory>
#include <mutex>
#include <string>
#include <gattlib.h>

//---------------------------------------------------------------------------
namespace jutta_bt_driver {
//---------------------------------------------------------------------------
struct ScanArgs {
    std::mutex m;
    std::string name;
    bool success{false};
    std::string addr;
} __attribute__((aligned(128)));

std::shared_ptr<BTCoffeeMaker> scan_for_device(std::string&& name);
//---------------------------------------------------------------------------
}  // namespace jutta_bt_driver
//---------------------------------------------------------------------------