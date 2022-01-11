//
// Created by alex on 06/01/2022.
//

#ifndef DRIVER_WIN_UTILS_H
#define DRIVER_WIN_UTILS_H

#include "BladeStructs.h"
#include <cstdio>
#include <chrono>

namespace librazerblade {
    class DllExport Utils
    {
    public:
        static void usleep(uint32_t us);

        static void printWarning(RazerPacket* report, const char* driver_name, const char* message);

        static uint8_t crc8(uint8_t* buffer, int length);

        static uint8_t crcPacket(RazerPacket* pkt);
    };
}


#endif //DRIVER_WIN_UTILS_H
