#pragma once

#include <blinkstickcpp_export.h>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

struct hid_device_;
using hid_device = hid_device_;

namespace blinkstick
{
    /**
     * @brief Possible blink stick types.
     */
    enum class device_type
    {
        unknown,
        basic,
        pro,
        square,
        strip,
        nano,
        flex
    };

    /**
     * @brief Possible blink stick modes (only valid for Blinkstick Pro).
     */
    enum class mode
    {
        unknown = -1,
        normal,
        inverse,
        smart_pixel
    };


    struct colour
    {
        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;
    };

    class BLINKSTICKCPP_EXPORT device
    {
    public:
        device(
            std::shared_ptr<hid_device> handle,
            device_type type);
        /**
         * @brief Sets the LED at the given index and channel to the specified color for the
         * provided device.
         * @param channel the channel to set the LED color on.
         * @param index which LED to set the color of.
         * @param red the red component of the new color.
         * @param green the green component of the new color.
         * @param blue the blue component of the new color.
         */
        bool set_colour(
            int channel,
            int index,
            uint8_t red,
            uint8_t green,
            uint8_t blue) const;

        bool set_colours(
            int channel,
            uint8_t red,
            uint8_t green,
            uint8_t blue) const;

        bool set_colours(
            int channel,
            const std::vector<colour>& colours) const;

        /**
         * @brief Reads the color from the blinkstick at a given index.
         * @param index the index of the LED to read from.
         * @return pointer to a color struct containing the read color.
         */
        colour get_colour(int index) const;

        /**
         * @brief Set the mode of the blinkstick.
         * @details Possible modes are "normal" (non-inverse LED control),
         * "inverse" (LED values are inverted) and "smart" (LEDs are WS2812 smart LEDs).
         * Note that you'll need to implement a delay after setting the mode before setting the
         * color on the blinkstick device.
         */
        bool set_mode(mode mode) const;

        /**
         * @brief Read the mode currently set on the blinkstick.
         * @return the current mode.
         */
        mode get_mode() const;

        /**
         * @brief Turns off the led at the specified index for the provided device.
         * This is the same as using set_color with the RGB value (0, 0, 0)
         */
        bool off(int channel, int index) const;

        bool off() const;

        /**
        * @brief Gets the number of leds on the device
        * @detail This will query the device on the first call, then cache the result so subsequent
        * calls not do query the device
        */
        int get_led_count() const;

        bool set_led_count(uint8_t count) const;

        device_type get_type() const;

        /**
        * @brief
        * @return Whether or not the device is valid
        */
        bool is_valid() const;

    private:
        std::shared_ptr<hid_device> handle;
        device_type type;
        mutable std::optional<int> led_count;
    };
}
