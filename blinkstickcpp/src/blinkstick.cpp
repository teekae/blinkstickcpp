#include "blinkstick/blinkstick.hpp"

#include <hidapi/hidapi.h>

#include <array>
#include <cstdarg>
#include <string>

namespace
{

    constexpr int VENDOR_ID = 0X20A0;
    constexpr int PRODUCT_ID = 0X41E5;

    bool print_debug = false;

    std::shared_ptr<hid_device> get_device(hid_device_info* device_info)
    {
        return std::shared_ptr<hid_device>(
            hid_open_path(device_info->path),
            [](hid_device* device)
			{
				if (device)
				{
					hid_close(device);
				}
			});
    }
}

namespace blinkstick
{
    void debug(const char* fmt, ...)
    {
        if (print_debug)
        {
            char buffer[256];
            va_list ap;
            va_start(ap, fmt);
            vsprintf(buffer, fmt, ap);
            va_end(ap);

            puts(buffer);
        }
    }

    void enable_logging()
    {
        print_debug = true;
        debug("STARTING BLINKSTICK WITH DEBUG LOGGING");
    }

    int get_major_version(hid_device_info* device_info)
    {
		if(device_info->serial_number == nullptr)
		{
			debug("No serial number");
			return 0;
		}
        const std::wstring serial = device_info->serial_number;
        try
        {
            return std::stoi(serial.substr(serial.size() - 3, 1));
        }
        catch (const std::exception&)
        {
			debug("Failed to parse serial number");
        }
        return 0;
    }

    device_type get_type(hid_device_info* device_info)
    {
        const auto major_version = get_major_version(device_info);

        if (major_version == 1)
        {
            return device_type::basic;
        }
        else if (major_version == 2)
        {
            return device_type::pro;
        }
        else if (major_version == 3)
        {
            switch (device_info->release_number)
            {
            case 0x0200:
                return device_type::square;
            case 0x0201:
                return device_type::strip;
            case 0x0202:
                return device_type::nano;
            case 0x0203:
                return device_type::flex;
            }
        }
        return device_type::unknown;
    }

    std::vector<device> find_all()
    {
        std::vector<device> devices;

        debug("initializing usb context");
        const int res = hid_init();
        if (res != 0)
        {
            debug("failed to initialize hid");
            return devices;
        }

        auto all_devices = hid_enumerate(VENDOR_ID, PRODUCT_ID);
        if (all_devices == nullptr)
        {
            return devices;
        }
        auto device_info = all_devices;

        do
        {
            if(auto device = get_device(device_info); !device)
            {
                debug("could not open device");
            }
            else
            {
                debug("found device: %s", device_info->path);
                devices.emplace_back(std::move(device), get_type(device_info));
            }
        } while ((device_info = device_info->next));

        hid_free_enumeration(all_devices);

        return devices;
    }

    device find()
    {
        const auto devices = find_all();
        if (devices.empty())
        {
            return device{ nullptr, device_type::unknown };
        }
        return devices.front();
    }

    void finalise()
    {
        hid_exit();
    }
}
