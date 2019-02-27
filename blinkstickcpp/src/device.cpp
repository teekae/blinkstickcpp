#include "blinkstick/device.h"

#include <hidapi/hidapi.h>

#include <array>

namespace
{
    constexpr int MODE_MSG_SIZE = 2;
    constexpr int COUNT_MSG_SIZE = 2;

    std::vector<uint8_t> build_control_message(const uint8_t index, const uint8_t channel, uint8_t red, uint8_t green, uint8_t blue)
    {
        // Write to the first LED present
        // this will be the _only_ led for the original blinkstick
        if (index == 0 && channel == 0)
        {
            return
            {
                0x1,
                red,
                green,
                blue
            };
        }

        // Writing to the other LEDs requires a different payload
        // this changes the write mode (first two bytes) and then
        // assigns the index.
        return
        {
            0x5,
            channel,
            index,
			red,
			green,
			blue
        };
    }

    constexpr std::array<uint8_t, MODE_MSG_SIZE> build_mode_message(blinkstick::mode mode)
    {
        return
        {
            0x0004,
            static_cast<uint8_t>(mode)
        };
    }

    constexpr std::array<uint8_t, COUNT_MSG_SIZE> build_count_message(const uint8_t count)
    {
        return
        {
            0x81,
            count
        };
    }

    std::pair<uint8_t, uint8_t> determine_report_id(const int count)
    {
        uint8_t max_leds = 64;
        uint8_t report_id = 9;

        if (count <= 8 * 3)
        {
            max_leds = 8;
            report_id = 6;
        }
        else if (count <= 16 * 3)
        {
            max_leds = 16;
            report_id = 7;
        }
        else if (count <= 32 * 3)
        {
            max_leds = 32;
            report_id = 8;
        }
        else if (count <= 64 * 3)
        {
            max_leds = 64;
            report_id = 9;
        }
        else if (count <= 128 * 3)
        {
            max_leds = 64;
            report_id = 10;
        }

        return { report_id, max_leds };
    }

    template<typename T>
    bool get_feature_report(const std::shared_ptr<hid_device>& handle, T& msg)
    {
        return hid_get_feature_report(handle.get(), msg.data(), msg.size()) != -1;
    }

    template<typename T>
    bool send_feature_report(const std::shared_ptr<hid_device>& handle, const T& msg)
    {
        return hid_send_feature_report(handle.get(), msg.data(), msg.size()) != -1;
    }
}

namespace blinkstick
{
    void debug(const char* fmt, ...);

    device::device(std::shared_ptr<hid_device> handle, device_type type) :
        handle(std::move(handle)),
        type(type)
    {
    }

    bool device::set_mode(const mode mode) const
    {
        if (handle == nullptr)
        {
            debug("input hid handle is null");
            return false;
        }

		const auto msg = build_mode_message(mode);

        if (!send_feature_report(handle, msg))
        {
            debug("error writing mode to device");
            return false;
        }
        return true;
    }

    mode device::get_mode() const
    {
        auto data = build_mode_message(mode::unknown);
        if (!send_feature_report(handle, data))
        {
            debug("error reading mode from device");
            return mode::unknown;
        }

        return static_cast<mode>(data[1]);
    }


    bool device::set_colour(
        const int channel,
        const int index,
        const uint8_t red,
        const uint8_t green,
        const uint8_t blue) const
    {
        if (handle == nullptr)
        {
            debug("input hid handle is null");
            return false;
        }
        const auto msg = build_control_message(index, channel, red, green, blue);
        if (!send_feature_report(handle, msg))
        {
            debug("error writing colour to device");
            return false;
        }
        return true;
    }

    bool device::set_colours(
        const int channel,
        const uint8_t red,
        const uint8_t green,
        const uint8_t blue) const
    {
        std::vector<colour> colours;
        const auto total_leds = get_led_count();
        colours.reserve(total_leds);
        for (int i = 0; i < total_leds; ++i)
        {
            colour colour;
            colour.red = red;
            colour.green = green;
            colour.blue = blue;
            colours.emplace_back(std::move(colour));
        }

        return set_colours(channel, colours);
    }

    bool device::set_colours(
        int channel,
        const std::vector<colour>& colours) const
    {
        if (handle == nullptr)
        {
            debug("input hid handle is null");
            return false;
        }

        const auto [report_id, max_leds] = determine_report_id(get_led_count() * 3);

        std::vector<uint8_t> msg(static_cast<size_t>(max_leds) * 3 + 2);

        msg[0] = report_id;
        msg[1] = channel;

        auto colourSize = std::min(static_cast<uint8_t>(colours.size()), max_leds);

        int index = 2;

        for (int i = 0; i < colourSize; ++i)
        {
            const auto& colour = colours[i];
            msg[index++] = colour.green;
            msg[index++] = colour.red;
            msg[index++] = colour.blue;
        }

        if (!send_feature_report(handle, msg))
        {
            debug("error writing colour to device");
            return false;
        }
        return true;
    }

    colour device::get_colour(const int index) const
    {
        colour color;
        color.red = 0;
        color.green = 0;
        color.blue = 0;

        if (index == 0)
        {
            std::array<uint8_t, 33> data;
            data[0] = 0x0001;

            if (!get_feature_report(handle, data))
            {
                debug("unable to read colour from blinkstick");
            }
            else
            {
                color.red = data[1];
                color.green = data[2];
                color.blue = data[3];
            }
        }
        else
        {
            const int count = (index + 1) * 3;
            const auto[report_id, max_leds] = determine_report_id(count);

            std::vector<uint8_t> data(static_cast<size_t>(max_leds) * 3 + 2);
            data[0] = report_id;

            if (!get_feature_report(handle, data))
            {
                debug("unable to read colour from blinkstick");
            }
            else
            {
                color.red = data[static_cast<size_t>(index) * 3 + 3];
                color.green = data[static_cast<size_t>(index) * 3 + 2];
                color.blue = data[static_cast<size_t>(index) * 3 + 4];
            }
        }
        return color;
    }


    bool device::off(const int channel, const int index) const
    {
        return set_colour(channel, index, 0, 0, 0);
    }

    bool device::off() const
    {
        return set_colours(0, 0, 0, 0);
    }

    int device::get_led_count() const
    {
        if (led_count)
        {
            return *led_count;
        }
        // Build a message with the default value of 0
        auto data = build_count_message(0);
        
        if (!get_feature_report(handle, data))
        {
            debug("error reading mode from device");
        }

        led_count = data[1];

        return *led_count;
    }

    bool device::set_led_count(const uint8_t count) const
    {
        if (handle == nullptr)
        {
            debug("input hid handle is null");
            return false;
        }

        const auto msg = build_count_message(count);

        if (!send_feature_report(handle, msg))
        {
            debug("error writing led count to device");
            return false;
        }
        led_count = count;
        return true;
    }

    device_type device::get_type() const
    {
        return type;
    }

    bool device::is_valid() const
    {
        return handle != nullptr;
    }
}
