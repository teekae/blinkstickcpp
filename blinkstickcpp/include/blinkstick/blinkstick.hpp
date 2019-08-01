#pragma once

#include <blinkstick/device.hpp>
#include <cstdint>
#include <vector>

namespace blinkstick
{

    /**
     * @brief Finds all BlinkStick devices
     * @return an vector of BlinkStick devices
     */
    std::vector<device> find_all();

    /**
     * @brief Find the first blinkstick device on the bus registered
     * with HID.
     * @return a BlinkStick device.
     */
    device find();

    /**
    * @brief Function to be called when all devices are no longer being used
    */
    void finalise();

    /**
     * @brief Turns on debug logging.
     */
    void enable_logging();
}
