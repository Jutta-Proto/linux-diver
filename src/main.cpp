#include "jutta_driver/JuttaDriver.hpp"
#include "logger/Logger.hpp"

int main(int /**argc**/, char** /**argv**/) {
    logger::setup_logger(spdlog::level::debug);
    SPDLOG_INFO("Launching Version: {} {}", JUTTA_DRIVER_VERSION, JUTTA_DRIVER_VERSION_NAME);

    jutta_driver::JuttaDriver driver("/dev/serial0");
    driver.run();
    return 0;
}
