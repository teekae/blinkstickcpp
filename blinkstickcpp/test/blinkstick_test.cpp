
#include <blinkstick/blinkstick.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <atomic>
#include <string>
#include <thread>

struct arguments
{
    int red;
    int green;
    int blue;
    int count;
    int index;
    int channel;
};

void usage()
{
    printf("USAGE:\n\
  blinkstick [options...]\n\
OPTIONS\n\
  --color set the color using a three rgb values\n\
  --count set the number of blinkstick devices to address\n\
  --index which led should be set\n\
  --channel which channel to use (BlinkStick Pro)\n\
  --debug turn on debug logging\n");
}

arguments parse_args(char** flags)
{
    arguments args;
    args.count = 1;
    int color_set = 0;

    for (int i = 1; i < sizeof(flags); i++)
    {
        if (flags[i] != NULL)
        {
            if (strcmp(flags[i], "--index") == 0)
            {
                args.index = atoi(*(flags + i + 1));
            }

            if (strcmp(flags[i], "--channel") == 0)
            {
                args.channel = atoi(*(flags + i + 1));
            }

            if (strcmp(flags[i], "--debug") == 0)
            {
                blinkstick::enable_logging();
            }

            if (strcmp(flags[i], "--color") == 0)
            {
                color_set = 1;
                args.red = atoi(*(flags + i + 1));
                args.green = atoi(*(flags + i + 2));
                args.blue = atoi(*(flags + i + 3));
            }

            if (strcmp(flags[i], "--help") == 0)
            {
                usage();
                exit(0);
            }
        }

        if (color_set == 0)
        {
            usage();
            exit(1);
        }
    }

    return args;
}

int main(int argc, char** argv)
{
    arguments args;
    args.red = 255;
    args.green = 255;
    args.blue = 0;
    args.index = 1;
    args.channel = 0;
    blinkstick::enable_logging();
    const auto device = blinkstick::find();
    if (!device.is_valid())
    {
        std::cout << "No connected BlinkStick\n";
        return 1;
    }

    int led_count = device.get_led_count();
    std::cout << "There are " << led_count << " leds\n";
    // set the color
    bool red = true;
    std::vector<blinkstick::colour> colours(led_count);
    for (int i = 0; i < led_count; ++i)
    {
        auto& colour = colours[i];

        if (red)
        {
            //colour.red = 255;
            colour.red = 255;
            colour.green = 215;
        }
        else
        {
            //colour.green = 255;
            colour.red = 64;
            colour.green = 219;
            colour.blue = 219;
        }
        red = !red;
    }

    std::atomic_bool running = true;

    auto thread = std::thread(
        [&]()
    {
        std::string c;
        std::cin >> c;
        running = false;
    });

    while (running)
    {
        device.set_colours(args.channel, colours);
        std::rotate(colours.begin(), colours.begin() + 1, colours.end());
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    thread.join();

    device.off();

    blinkstick::finalise();

    return 0;
}
