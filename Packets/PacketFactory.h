//
// Created by alex on 08/01/2022.
//

#ifndef LIBRAZERBLADE_PACKETFACTORY_H
#define LIBRAZERBLADE_PACKETFACTORY_H

#include <cstring>
#include "../BladeStructs.h"

namespace librazerblade {
    class DllExport PacketFactory
    {
    public:
        static uint8_t getDataSize(BladePacketType type);

        static RazerPacket empty();

        static RazerPacket
        createRazerPacket(uint8_t command_class, BladePacketType type, BladePacketDirection direction = HostToDevice,
                          uint8_t size = 0);

        static RazerPacket fan(uint8_t fanSpeedDiv100, int fanId, BladePacketDirection direction);

        static RazerPacket power(uint8_t powerMode, bool autoFanSpeed, BladePacketDirection direction = Set);

        static RazerPacket row(KeyboardRow row, BladePacketDirection direction = Set);

        static RazerPacket applyChroma();

        static RazerPacket brightness(uint8_t brightness, BladePacketDirection direction = Set);
    };
}

#endif //LIBRAZERBLADE_PACKETFACTORY_H
