#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

//---------------------------------------------------------------------------
namespace jutta_bt_driver {
//---------------------------------------------------------------------------
std::vector<uint8_t> deobfuscate(const std::vector<uint8_t>& data, uint8_t key);
//---------------------------------------------------------------------------
}  // namespace jutta_bt_driver
//---------------------------------------------------------------------------