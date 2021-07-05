#include "LogitechLEDLib.h"
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>

#pragma comment(lib, "LogitechLEDLib.lib")

inline static std::array<int, 3> loadColorProfile(std::string_view name)
{
    std::ifstream fs{std::string{name} + ".txt"};

    int r, g, b;
    fs >> std::hex >> r >> g >> b;

    return {r, g, b};
}

inline static int toPercent(int val)
{
    val *= 100.0 / 255.0;
    return val > 100 ? 100 : val;
}

int main(int argc, char** argv)
{
    int args[3] = {0, 0, 0};
    int delay = 1;
    if(argc == 5)
    {
        args[0] = toPercent(atoi(argv[1]));
        args[1] = toPercent(atoi(argv[2]));
        args[2] = toPercent(atoi(argv[3]));
        delay = atoi (argv[4]);
    }
    if(argc == 3)
    {
        auto [r, g, b] = loadColorProfile(argv[1]);

        args[0] = toPercent(r);
        args[1] = toPercent(g);
        args[2] = toPercent(b);
        delay = atoi(argv[2]);
    }
    if(!LogiLedInit())
    {
        std::cout << "Initialisation failed" << std::endl;
        return -1;
    }
    /* clang-format off */
    std::cout << "Initialisation successful\t" << args[0] << '\t'
                                               << args[1] << '\t'
                                               << args[2] << std::endl;
    /* clang-format on */

    LogiLedSetTargetDevice(LOGI_DEVICETYPE_ALL);
    LogiLedSetLighting(args[0], args[1], args[2]);

    std::this_thread::sleep_for(std::chrono::seconds{delay});
    LogiLedShutdown();

    return 1;
}
