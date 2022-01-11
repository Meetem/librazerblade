//
// Created by alex on 08/01/2022.
//

#include "UsbInterface.h"

namespace librazerblade {
    UsbInterface gUsb;

    UsbInterface::UsbInterface() noexcept:
            implementation()
    {

    }

    int32_t
    UsbInterface::controlTransfer(void* handle, uint32_t requestType, uint32_t request, uint32_t value,
                                  uint32_t reportIndex,
                                  uint8_t* buffer, uint16_t bufferSize, uint32_t timeout, void* aux)
    {

        std::lock_guard<std::mutex> autoLock(callLock);
        if (implementation.controlTransfer == nullptr)
            return INT32_MIN;

        return implementation.controlTransfer(handle, requestType, request, value, reportIndex, buffer, bufferSize,
                                              timeout,
                                              aux);
    }

    int32_t
    UsbInterface::sendControlMessage(void* handle, uint8_t* buffer, uint16_t size, uint32_t timeout,
                                     uint32_t delayAfter,
                                     void* aux)
    {
        std::lock_guard<std::mutex> autoLock(callLock);
        if (implementation.sendControlMessage == nullptr)
            return INT32_MIN;

        return implementation.sendControlMessage(handle, buffer, size, timeout, delayAfter, aux);
    }

    int32_t UsbInterface::getUsbResponse(void* handle, RazerPacket* req_buffer, RazerPacket* resp_buffer,
                                         unsigned long maxWait, void* aux)
    {
        uint32_t request = 0x01; // 0x01
        uint32_t request_type = 0xA1;//USB_TYPE_CLASS | USB_RECIP_INTERFACE | USB_DIR_IN; // 0xA1
        uint32_t value = 0x300;
        uint32_t response_index = 0x02;
        uint32_t size = RAZER_USB_REPORT_LEN;

        auto* buf = (uint8_t*) malloc(size);
        if (buf == nullptr)
            return -ENOMEM;

        memset(buf, 0, size);

        // Send request to device
        sendControlMessage(handle, (uint8_t*) req_buffer, size, DEFAULT_TIMEOUT, maxWait, aux);

        int len = controlTransfer(handle, request_type, request, value, response_index, buf, size, DEFAULT_TIMEOUT,
                                  aux);

        if(len == size)
            memcpy(resp_buffer, buf, size);

        free(buf);

        if (len != size) {
            return -EIO;
        }

        return 0;
    }

    RazerPacket
    UsbInterface::sendPayload(void* handle, RazerPacket* request_report, uint32_t delayAfter, UsbPacketResult* result,
                              void* aux)
    {
        request_report->crc = Utils::crcPacket(request_report);
        /*
        printf("Sending payload: 0x%02X, data size %02X (%d)\n",
               request_report->command_id.id,
               request_report->data_size, request_report->data_size);
        */

        RazerPacket response_report = {0};
        response_report.status = RazerResponseDoesntMatch;

        auto retval = getUsbResponse(handle, request_report, &response_report, delayAfter,
                                     aux);

        BladeResponse response = RazerResponseFailure;

        if (retval == 0) {
            // Check the packet number, class and command are the same
            if (response_report.remaining_packets != request_report->remaining_packets ||
                response_report.command_class != request_report->command_class ||
                response_report.command_id.id != request_report->command_id.id ||
                response_report.transaction_id.id != request_report->transaction_id.id) {
                // This can occur if we wire too many commands at once. Ignore
                //Utils::printWarning(&response_report, "Razer laptop control", "Response doesn't match request");
                response = RazerResponseDoesntMatch;
            } else {
                response = (BladeResponse) (response_report.status);
            }
        }

        if (result != nullptr) {
            *result = {retval, response,
                       (retval == 0 && response == RazerResponseSuccess) ? 1 : 0};
        }

        return response_report;
    }

    void UsbInterface::freeDeviceList(UsbDeviceList* list)
    {
        if (implementation.freeDeviceList == nullptr)
            return;

        return implementation.freeDeviceList(list);
    }

    UsbDeviceList* UsbInterface::listDevices()
    {
        if (implementation.listDevices == nullptr)
            return nullptr;

        return implementation.listDevices();
    }

    void* UsbInterface::openDevice(UsbDevice* device)
    {
        if (implementation.openDevice == nullptr)
            return nullptr;

        return implementation.openDevice(device);
    }

    void UsbInterface::closeHandle(void* handle, UsbDevice* device)
    {
        if (implementation.closeHandle == nullptr)
            return;

        implementation.closeHandle(handle, device);
    }

    void UsbInterface::setImplementation(UsbInterfaceImplementation impl)
    {
        std::lock_guard<std::mutex> autoLock(callLock);
        implementation = impl;
    }

    UsbInterfaceImplementation UsbInterface::getImplementation() const
    {
        return implementation;
    }
}