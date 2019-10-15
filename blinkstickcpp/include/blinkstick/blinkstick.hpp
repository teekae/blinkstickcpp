#pragma once

#include <blinkstick/device.hpp>
#include <blinkstickcpp_export.h>
#include <cstdint>
#include <vector>

namespace blinkstick
{

    /**
     * @brief Finds all BlinkStick devices
     * @return an vector of BlinkStick devices
     */
    std::vector<device> BLINKSTICKCPP_EXPORT find_all();

    /**
     * @brief Find the first blinkstick device on the bus registered
     * with HID.
     * @return a BlinkStick device.
     */
    device BLINKSTICKCPP_EXPORT find();

    /**
    * @brief Function to be called when all devices are no longer being used
    */
    void BLINKSTICKCPP_EXPORT finalise();

    /**
     * @brief Turns on debug logging.
     */
    void BLINKSTICKCPP_EXPORT enable_logging();
}
