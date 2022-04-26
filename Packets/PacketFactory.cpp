//
// Created by alex on 08/01/2022.
//

#include "PacketFactory.h"

namespace librazerblade {
    uint8_t PacketFactory::getDataSize(BladePacketType type)
    {
        switch (type) {
            case BladePacketType::PktFan:
                return 0x03;
            case BladePacketType::PktBoostMode:
                return 0x03;
            case BladePacketType::PktPower:
                return 0x04;
            case BladePacketType::PktGetBrightness:
                return 0x03;
            case BladePacketType::PktSetBrightness:
                return 0x03;
            case BladePacketType::PktChromaApply:
                return 0x02;
            case BladePacketType::PktChromaSetRow:
                return 0x34;
        }

        return 0x00;
    }

    RazerPacket PacketFactory::empty()
    {
        return RazerPacket{0};
    }

    RazerPacket
    PacketFactory::createRazerPacket(uint8_t command_class, BladePacketType type, BladePacketDirection direction,
                                     uint8_t size)
    {
        RazerPacket new_report = {0};
        memset(&new_report, 0, sizeof(RazerPacket));

        if (size == 0x00)
            size = getDataSize(type);

        new_report.status = 0x00;
        new_report.transaction_id.id = 0x1F;
        new_report.remaining_packets = 0x00;
        new_report.protocol_type = 0x00;
        new_report.command_class = command_class;
        new_report.command_id.parts.direction = direction & 0x01;
        new_report.command_id.parts.id = type;
        new_report.data_size = size;
        return new_report;
    }

    RazerPacket PacketFactory::boost(BladeBoostId boostId, uint8_t mode, BladePacketDirection direction)
    {
        if(direction == Get)
            mode = 0;

        RazerPacket pkt = createRazerPacket(0x0d, PktBoostMode, direction);
        pkt.args[0] = 0x00;
        pkt.args[1] = (uint8_t)boostId;
        pkt.args[2] = mode;
        return pkt;
    }

    RazerPacket PacketFactory::fan(uint8_t fanSpeedDiv100, int fanId, BladePacketDirection direction)
    {
        if (direction == Get)
            fanSpeedDiv100 = 0;

        RazerPacket pkt = createRazerPacket(0x0d, PktFan, direction);
        pkt.args[0] = 0x00;
        pkt.args[1] = fanId;
        pkt.args[2] = fanSpeedDiv100;
        return pkt;
    }

    RazerPacket PacketFactory::power(uint8_t powerMode, bool autoFanSpeed, BladePacketDirection direction)
    {
        uint8_t aux = 0x01;
        if (direction == Get) {
            powerMode = 0;
            autoFanSpeed = false;
            aux = 0x02;
        }

        RazerPacket pkt = createRazerPacket(0x0d, PktPower, direction);
        pkt.args[0] = 0x00;
        pkt.args[1] = aux;
        pkt.args[2] = powerMode;
        pkt.args[3] = autoFanSpeed ? 0 : 1;
        return pkt;
    }

    RazerPacket PacketFactory::row(KeyboardRow row, BladePacketDirection direction)
    {
        RazerPacket pkt = createRazerPacket(0x03, PktChromaSetRow, direction);
        pkt.args[0] = 0xff;
        pkt.args[1] = row.rowid;
        pkt.args[3] = 0x0f;

        if (direction == Set)
            memcpy(&pkt.args[7], &row.keys, 45);

        return pkt;
    }

    RazerPacket PacketFactory::applyChroma()
    {
        RazerPacket pkt = createRazerPacket(0x03, PktChromaApply, Set);
        pkt.args[0] = 0x05;
        pkt.args[1] = 0x00;
        return pkt;
    }

    RazerPacket PacketFactory::brightness(uint8_t brightness, BladePacketDirection direction)
    {
        if (direction == Set) {
            auto pkt = createRazerPacket(0x0E, PktSetBrightness, Set);
            pkt.args[0] = 0x01;
            pkt.args[1] = brightness;
            pkt.args[2] = 0x00;
            return pkt;
        }

        auto pkt = createRazerPacket(0x03, PktGetBrightness, Get);
        pkt.args[0] = 0x01;
        pkt.args[1] = 0x05;
        pkt.args[2] = 0x00;
        return pkt;
    }
}