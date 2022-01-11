//
// Created by alex on 08/01/2022.
//

#ifndef LIBRAZERBLADE_USBINTERFACE_H
#define LIBRAZERBLADE_USBINTERFACE_H

#include "../BladeStructs.h"
#include "../UsbInterfaceStructs.h"
#include "../Utils.h"

#include <mutex>

namespace librazerblade {
    class DllExport UsbInterface
    {
    public:
        UsbInterface() noexcept;

        void setImplementation(UsbInterfaceImplementation impl);

        /**
         * Sends payload to the EC controller
         * @param buffer Payload buffer
         * @param size Size of the payload buffer
         * @param maxWait Maximum time to wait in us after sending the payload
         */
        int32_t
        sendControlMessage(void* handle, uint8_t* buffer, uint16_t size, uint32_t timeout = 5000,
                           uint32_t delayAfter = 800,
                           void* aux = nullptr);

        int32_t
        controlTransfer(void* handle, uint32_t requestType, uint32_t request, uint32_t value, uint32_t reportIndex,
                        uint8_t* buffer, uint16_t bufferSize, uint32_t timeout, void* aux);

        RazerPacket sendPayload(void* handle, RazerPacket* request_report, uint32_t delayAfter = 1000,
                                UsbPacketResult* result = nullptr, void* aux = nullptr);

        int32_t getUsbResponse(void* handle, RazerPacket* req_buffer, RazerPacket* resp_buffer,
                               unsigned long maxWait = 1000, void* aux = nullptr);

        void closeHandle(void* handle, UsbDevice* device);

        void* openDevice(UsbDevice* device);

        UsbDeviceList* listDevices();

        void freeDeviceList(UsbDeviceList* list);

        UsbInterfaceImplementation getImplementation() const;

    protected:
        std::mutex callLock;
        UsbInterfaceImplementation implementation;
    };

    extern UsbInterface gUsb;
}

#endif //LIBRAZERBLADE_USBINTERFACE_H
