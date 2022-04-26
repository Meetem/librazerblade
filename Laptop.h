//
// Created by alex on 09/01/2022.
//

#ifndef LIBRAZERBLADE_LAPTOP_H
#define LIBRAZERBLADE_LAPTOP_H

#include "BladeStructs.h"
#include "LaptopDescription.h"
#include "Usb/UsbInterface.h"
#include "Packets/PacketFactory.h"
#include "Packets/PacketUtil.h"
#include "mutex"

typedef std::lock_guard<std::mutex> LockGuard;

namespace librazerblade {
    class DllExport Laptop
    {
    public:
        Laptop(LaptopDescription description, UsbHandle usbHandle, UsbDevice device);
        ~Laptop();

        UsbDevice getDevice() const;
        UsbHandle getUsbHandle() const;

        void setUsbHandle(UsbHandle handle);
        uint8_t clampFan(int32_t v) const;
        BladeCapabilities getCapabilities() const;
        LaptopDescription getDescription();
        LaptopState getState();

        //Unsafe.
        LaptopState* getStatePtrUnsafe();
        UsbPacketResult queryFanSpeed(int numRetries = 0);
        UsbPacketResult queryPowerMode(int numRetries = 0);
        UsbPacketResult queryBrightness(int numRetries = 0);
        UsbPacketResult queryChromaRow(int rowId, int numRetries = 0);
        LaptopQueryResult query(BladeQuery query, int numRetries = 0);
        LaptopQueryResult queryRows(BladeQueryRows rows, int numRetries = 0);
        UsbPacketResult setFanSpeed(int32_t speed, int numRetries = 0, int fanId = 1, bool clampSpeed = true);
        UsbPacketResult setPowerMode(uint8_t powerMode, bool autoFanSpeed, int numRetries = 0);
        UsbPacketResult setBrightness(uint8_t brightness, int numRetries = 0);
        UsbPacketResult applyChroma(int numRetries = 0);
        UsbPacketResult sendKeyboardRow(KeyboardRow row, int numRetries = 0);
        UsbPacketResult
        sendPacketWithRetry(RazerPacket* packet, RazerPacket* output, int numRetries = 0, int retryIntervalMs = 250);

        static int getArrayIndex(int32_t q);

    protected:
        std::mutex queryMutex;
        std::mutex laptopMutex;
        LaptopDescription description;
        LaptopState state;
        UsbHandle usbHandle;
        UsbDevice device;
    };
}

#endif //LIBRAZERBLADE_LAPTOP_H
