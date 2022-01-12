//
// Created by alex on 09/01/2022.
//

#include "Laptop.h"

namespace librazerblade {

    Laptop::Laptop(LaptopDescription description, UsbHandle usbHandle, UsbDevice device) :
            description(description),
            usbHandle(usbHandle),
            device(device),
            state{0}
    {

    }

    UsbDevice Laptop::getDevice() const
    {
        return device;
    }

    UsbHandle Laptop::getUsbHandle() const
    {
        return usbHandle;
    }

    void Laptop::setUsbHandle(UsbHandle v)
    {
        LockGuard autoLockq(queryMutex);
        LockGuard lock(laptopMutex);
        this->usbHandle = v;
    }

    uint8_t Laptop::clampFan(int32_t v) const
    {
        if (v <= 0)
            return 0;

        v = std::max(std::min(v, description.fan.maxFanSpeed), description.fan.minFanSpeed);
        return v / 100;
    }

    BladeCapabilities Laptop::getCapabilities() const
    {
        return description.capabilities;
    }

    LaptopDescription Laptop::getDescription()
    {
        LockGuard autoLockq(queryMutex);
        LockGuard autoLock(laptopMutex);
        return description;
    }

    LaptopState Laptop::getState()
    {
        LockGuard autoLockq(queryMutex);
        LockGuard autoLock(laptopMutex);
        return state;
    }

    LaptopState* Laptop::getStatePtrUnsafe()
    {
        LockGuard autoLockq(queryMutex);
        LockGuard autoLock(laptopMutex);
        return &state;
    }

    UsbPacketResult Laptop::queryFanSpeed(int numRetries)
    {
        LockGuard autoLock(laptopMutex);
        auto pkt = PacketFactory::fan(0, Get);
        auto output = PacketFactory::empty();
        auto result = sendPacketWithRetry(&pkt, &output, numRetries);

        if (result.isSuccess) {
            state.fanSpeed = PacketUtil::getFanValueRaw(&output);
        }

        return result;
    }

    UsbPacketResult Laptop::queryPowerMode(int numRetries)
    {
        LockGuard autoLock(laptopMutex);
        auto pkt = PacketFactory::power(0, false, Get);
        auto output = PacketFactory::empty();
        auto result = sendPacketWithRetry(&pkt, &output, numRetries);

        if (result.isSuccess) {
            state.powerMode = PacketUtil::getPowerMode(&output);
            state.manualFanSpeed = PacketUtil::getManualFanSpeed(&output);
        }

        return result;
    }

    UsbPacketResult Laptop::queryBrightness(int numRetries)
    {
        LockGuard autoLock(laptopMutex);
        auto pkt = PacketFactory::brightness(0, Get);
        auto output = PacketFactory::empty();
        auto result = sendPacketWithRetry(&pkt, &output, numRetries);

        if (result.isSuccess) {
            state.keyboardInfo.brightness = PacketUtil::getBrightness(&output);
        }

        return result;
    }

    UsbPacketResult Laptop::queryChromaRow(int rowId, int numRetries)
    {
        LockGuard autoLock(laptopMutex);
        KeyboardRow row;
        row.rowid = rowId;

        auto pkt = PacketFactory::row(row, Get);
        auto output = PacketFactory::empty();
        auto result = sendPacketWithRetry(&pkt, &output, numRetries);

        if (result.isSuccess) {
            row = PacketUtil::getRow(&output);

            if (row.rowid != rowId) {
                result.isSuccess = 0;
                result.sendResult = RazerResponseDoesntMatch;
                return result;
            }

            state.keyboardInfo.rows[rowId] = row;
        }

        return result;
    }

    LaptopQueryResult Laptop::query(BladeQuery query, int numRetries)
    {
        LockGuard autoLock(queryMutex);

        LaptopQueryResult result{0};
        if (query & BladeQuery::QueryBrightness) {
            result.results[getArrayIndex(BladeQuery::QueryBrightness)] = queryBrightness(numRetries);
        }

        if (query & BladeQuery::QueryFanSpeed) {
            result.results[getArrayIndex(BladeQuery::QueryFanSpeed)] = queryFanSpeed(numRetries);
        }

        if (query & BladeQuery::QueryPowerMode) {
            result.results[getArrayIndex(BladeQuery::QueryPowerMode)] = queryPowerMode(numRetries);
        }

        return result;
    }

    LaptopQueryResult Laptop::queryRows(BladeQueryRows rows, int numRetries)
    {
        LockGuard autoLock(queryMutex);
        LaptopQueryResult result{0};

        for (int i = 0; i < BladeQueryRows::QueryRowCount; i++) {
            auto check = (BladeQueryRows) (1 << i);
            if ((rows & check) == 0)
                continue;

            result.results[i] = queryChromaRow(i, numRetries);
        }

        return result;
    }

    UsbPacketResult Laptop::setFanSpeed(int32_t speed, int numRetries)
    {
        auto clamped = this->clampFan(speed);
        auto pkt = PacketFactory::fan(clamped);
        auto output = PacketFactory::empty();

        auto r = sendPacketWithRetry(&pkt, &output, numRetries);
        if (r.isSuccess) {
            state.fanSpeed = clamped;
        }

        return r;
    }

    UsbPacketResult Laptop::setPowerMode(uint8_t powerMode, bool autoFanSpeed, int numRetries)
    {
        if (powerMode > 4)
            powerMode = 0;

        auto pkt = PacketFactory::power(powerMode, autoFanSpeed);
        auto output = PacketFactory::empty();

        auto r = sendPacketWithRetry(&pkt, &output, numRetries);
        if (r.isSuccess) {
            state.powerMode = powerMode;
            state.manualFanSpeed = autoFanSpeed ? 0 : 1;
        }

        return r;
    }

    UsbPacketResult Laptop::setBrightness(uint8_t brightness, int numRetries)
    {
        auto pkt = PacketFactory::brightness(brightness);
        auto output = PacketFactory::empty();

        auto r = sendPacketWithRetry(&pkt, &output, numRetries);
        if (r.isSuccess) {
            state.keyboardInfo.brightness = brightness;
        }

        return r;
    }

    UsbPacketResult Laptop::applyChroma(int numRetries)
    {
        auto pkt = PacketFactory::applyChroma();
        auto output = PacketFactory::empty();

        auto r = sendPacketWithRetry(&pkt, &output, numRetries);
        return r;
    }

    UsbPacketResult Laptop::sendKeyboardRow(KeyboardRow row, int numRetries)
    {
        if (row.rowid > 5)
            row.rowid = 0;

        auto pkt = PacketFactory::row(row);
        auto output = PacketFactory::empty();
        auto r = sendPacketWithRetry(&pkt, &output, numRetries);

        if (r.isSuccess) {
            state.keyboardInfo.rows[row.rowid] = row;
        }

        return r;
    }

    int Laptop::getArrayIndex(int32_t q)
    {
        int idx = 0;

        while ((q & 0x01) == 0) {
            idx++;
            q >>= 1;
        }

        return idx;
    }

    UsbPacketResult
    Laptop::sendPacketWithRetry(RazerPacket* packet, RazerPacket* output, int numRetries, int retryIntervalMs)
    {
        UsbPacketResult result{};

        do {
            auto pktResult = gUsb.sendPayload(usbHandle.handle, packet, 1000, &result, device.aux);
            if (output != nullptr)
                *output = pktResult;

            if (result.isSuccess > 0) {
                break;
            }

            numRetries--;
            Utils::usleep(retryIntervalMs * 1000);
        } while (numRetries > 0);

        return result;
    }

    Laptop::~Laptop()
    {
        if(usbHandle.autoRelease){
            usbHandle.autoRelease = false;
            gUsb.closeHandle(usbHandle.handle, &device);
        }
    }
}