//
// Created by alex on 09/01/2022.
//

#ifndef LIBRAZERBLADE_LAPTOPDESCRIPTION_H
#define LIBRAZERBLADE_LAPTOPDESCRIPTION_H

#include "BladeStructs.h"
#include <cinttypes>

#define DEFAULT_MIN 3100
#define DEFAULT_MAX 5300

enum BladeCapabilities
{
    None = 0x00,
    Boost = 0x01,
    Creator = 0x02,
    All = 0x7FFFFFFF
};

typedef struct LaptopFan_s
{
    int32_t minFanSpeed;
    int32_t maxFanSpeed;
} LaptopFan;

typedef struct LaptopDescription_s
{
    uint16_t vendorId;
    uint16_t productId;
    char name[256];
    LaptopFan fan;
    BladeCapabilities capabilities;
    UserData userData;
} LaptopDescription;

typedef struct LaptopState_s
{
    uint8_t fanSpeed;
    uint8_t powerMode;
    uint8_t manualFanSpeed;
    KeyboardInfo keyboardInfo;
} LaptopState;

#endif //LIBRAZERBLADE_LAPTOPDESCRIPTION_H
