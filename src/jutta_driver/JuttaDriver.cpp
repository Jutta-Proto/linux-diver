#include "JuttaDriver.hpp"
#include "jutta_proto/JuttaConnection.hpp"
#include "logger/Logger.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

//---------------------------------------------------------------------------
namespace jutta_driver {
//---------------------------------------------------------------------------
JuttaDriver::JuttaDriver(std::string&& device) : connection(std::move(device)) {}

void JuttaDriver::run() {
}
//---------------------------------------------------------------------------
}  // namespace jutta_driver
//---------------------------------------------------------------------------
