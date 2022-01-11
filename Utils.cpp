//
// Created by alex on 08/01/2022.
//

#include "Utils.h"

namespace librazerblade {
    void Utils::usleep(uint32_t us)
    {
        using namespace std;
        using namespace chrono;

        auto start = high_resolution_clock::now();
        int i = 0;
        while (duration_cast<microseconds>(high_resolution_clock::now() - start).count() < us) {
            i++;
        }
    }

    void Utils::printWarning(RazerPacket* report, const char* driver_name, const char* message)
    {
        printf("Warning: %s: %s. Start Marker: %02x id: %02x Num Params: %02x Reserved: %02x Command: %02x Params: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x .\n",
               driver_name,
               message,
               report->status,
               report->transaction_id.id,
               report->data_size,
               report->command_class,
               report->command_id.id,
               report->args[0], report->args[1], report->args[2], report->args[3], report->args[4], report->args[5],
               report->args[6], report->args[7], report->args[8], report->args[9], report->args[10], report->args[11],
               report->args[12], report->args[13], report->args[14], report->args[15]);
    }

    uint8_t Utils::crc8(uint8_t* buffer, int length)
    {
        uint8_t res = 0;
        // Simple CRC. Iterate over all bits from 2-87 and XOR them together
        for (int i = 0; i < length; i++)
            res ^= buffer[i];

        return res;
    }

    uint8_t Utils::crcPacket(RazerPacket* pkt)
    {
        uint8_t* ptr = (uint8_t*) pkt;
        ptr += 2;
        return crc8(ptr, 88 - 2);
    }
}