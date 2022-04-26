//
// Created by alex on 10/01/2022.
//

#include "PacketUtil.h"
#include <memory>

namespace librazerblade {
    uint8_t PacketUtil::getBoostValue(RazerPacket* pkt)
    {
        if (pkt == nullptr)
            return 0;

        return pkt->args[2];
    }

    uint8_t PacketUtil::getFanValueRaw(RazerPacket* pkt)
    {
        if (pkt == nullptr)
            return 0;

        return pkt->args[2];
    }

    uint8_t PacketUtil::getPowerMode(RazerPacket* pkt)
    {
        if (pkt == nullptr)
            return 0;

        return pkt->args[2];
    }

    uint8_t PacketUtil::getManualFanSpeed(RazerPacket* pkt)
    {
        if (pkt == nullptr)
            return 0;

        return pkt->args[3];
    }

    int32_t PacketUtil::getFanValue(RazerPacket* pkt)
    {
        return getFanValueRaw(pkt) * 100;
    }

    uint8_t PacketUtil::getBrightness(RazerPacket* pkt)
    {
        if (pkt == nullptr)
            return 0;

        if(pkt->command_id.parts.direction == 1)
            return pkt->args[2];

        return pkt->args[1];
    }

    KeyboardRow PacketUtil::getRow(RazerPacket* pkt)
    {
        KeyboardRow row = {};
        if (pkt == nullptr)
            return row;

        row.rowid = pkt->args[1];
        memcpy(&row.keys, &pkt->args[7], 45);
        return row;
    }
}