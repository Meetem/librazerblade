//
// Created by alex on 08/01/2022.
//

#ifndef LIBRAZERBLADE_PACKETUTIL_H
#define LIBRAZERBLADE_PACKETUTIL_H

#include "../BladeStructs.h"

namespace librazerblade {
    class DllExport PacketUtil
    {
    public:
        static uint8_t getFanValueRaw(RazerPacket* pkt);
        static uint8_t getPowerMode(RazerPacket* pkt);
        static int32_t getFanValue(RazerPacket* pkt);
        static uint8_t getBrightness(RazerPacket* pkt);
        static uint8_t getManualFanSpeed(RazerPacket* pkt);
        static KeyboardRow getRow(RazerPacket* pkt);
        static uint8_t getBoostValue(RazerPacket* pkt);
    };
}
#endif //LIBRAZERBLADE_PACKETUTIL_H
