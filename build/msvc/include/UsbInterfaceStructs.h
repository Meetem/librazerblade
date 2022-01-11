//
// Created by alex on 08/01/2022.
//

#ifndef LIBRAZERBLADE_USBINTERFACESTRUCTS_H
#define LIBRAZERBLADE_USBINTERFACESTRUCTS_H

typedef struct UsbDevice_s
{
    void* device;
    void* aux;
    uint16_t vendorId;
    uint16_t productId;
} UsbDevice;

typedef struct UsbDeviceList_s
{
    int32_t count;
    int32_t error;
    UsbDevice_s* devices;
    void* userData;
} UsbDeviceList;

typedef int32_t (* controlTransferFunction)(void* handle, uint32_t requestType, uint32_t request, uint32_t value,
                                            uint32_t reportIndex, uint8_t* buffer, uint16_t bufferSize,
                                            uint32_t timeout,
                                            void* aux);

typedef int32_t (* sendControlMessageFunction)(void* handle, uint8_t* buffer, uint16_t size, uint32_t timeout,
                                               uint32_t delayAfter, void* aux);

typedef void (* closeHandleFunction)(void* handle, UsbDevice* device);
typedef void* (* openDeviceFunction)(UsbDevice* device);
typedef UsbDeviceList* (* listDevicesFunction)();
typedef void (* freeDeviceListFunction)(UsbDeviceList* list);

typedef struct UsbInterfaceImplementation_s
{
    controlTransferFunction controlTransfer;
    sendControlMessageFunction sendControlMessage;
    closeHandleFunction closeHandle;
    openDeviceFunction openDevice;
    listDevicesFunction listDevices;
    freeDeviceListFunction freeDeviceList;
} UsbInterfaceImplementation;

typedef struct UsbPacketResult_s
{
    int32_t sendResult;
    int32_t packetResult;
    int32_t isSuccess;
} UsbPacketResult;

typedef struct LaptopQueryResult_s
{
    UsbPacketResult results[8];
} LaptopQueryResult;

#endif //LIBRAZERBLADE_USBINTERFACESTRUCTS_H
