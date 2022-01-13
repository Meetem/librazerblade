//
// Created by alex on 08/01/2022.
//

#ifndef LIBRAZERBLADE_LIBRAZERBLADE_H
#define LIBRAZERBLADE_LIBRAZERBLADE_H

#include "BladeStructs.h"
#include "UsbInterfaceStructs.h"
#include "LaptopDescription.h"

typedef void* LaptopPtr;

#ifdef __cplusplus
extern "C" {
#endif

//#region Usb Interface
DllExport int32_t CallType librazerblade_initialize();
DllExport void CallType librazerblade_deinitialize();

DllExport void
CallType librazerblade_UsbInterface_setImplementation(UsbInterfaceImplementation anImplementation);
DllExport UsbInterfaceImplementation CallType librazerblade_UsbInterface_getImplementation();
DllExport int32_t CallType
librazerblade_UsbInterface_controlTransfer(void* handle, uint32_t requestType, uint32_t request, uint32_t value,
                                           uint32_t reportIndex,
                                           uint8_t* buffer, uint16_t bufferSize, uint32_t timeout, void* aux);

DllExport RazerPacket
CallType librazerblade_UsbInterface_sendPayload(void* handle, RazerPacket* request, uint32_t delayAfter,
                                                UsbPacketResult* result, void* aux);

DllExport int32_t
CallType librazerblade_UsbInterface_getUsbResponse(void* handle, RazerPacket* request,
                                                   RazerPacket* responseBuffer,
                                                   unsigned long maxWait, void* aux);

DllExport void CallType librazerblade_UsbInterface_closeHandle(void* handle, UsbDevice* device);

DllExport void* CallType librazerblade_UsbInterface_openDevice(UsbDevice* device);

DllExport UsbDeviceList* CallType librazerblade_UsbInterface_listDevices();

DllExport void CallType librazerblade_UsbInterface_freeDeviceList(UsbDeviceList* list);
//#endregion

//#region Packet Factory
DllExport uint8_t CallType librazerblade_PacketFactory_getDataSize(BladePacketType type);

DllExport RazerPacket CallType librazerblade_PacketFactory_empty();

DllExport RazerPacket CallType
librazerblade_PacketFactory_createRazerPacket(uint8_t command_class, BladePacketType type,
                                              BladePacketDirection direction = HostToDevice,
                                              uint8_t size = 0);

DllExport RazerPacket
CallType librazerblade_PacketFactory_fan(uint8_t fanSpeedDiv100, BladePacketDirection direction = Set);

DllExport RazerPacket
CallType librazerblade_PacketFactory_power(uint8_t powerMode, int32_t autoFanSpeed, BladePacketDirection direction = Set);

DllExport RazerPacket CallType librazerblade_PacketFactory_row(KeyboardRow row, BladePacketDirection direction = Set);

DllExport RazerPacket CallType librazerblade_PacketFactory_applyChroma();

DllExport RazerPacket
CallType librazerblade_PacketFactory_brightness(uint8_t brightness, BladePacketDirection direction = Set);
//#endregion

//#region Packet Util
DllExport uint8_t CallType librazerblade_PacketUtil_getFanValueRaw(RazerPacket* pkt);
DllExport int32_t CallType librazerblade_PacketUtil_getFanValue(RazerPacket* pkt);
DllExport uint8_t CallType librazerblade_PacketUtil_getManualFanSpeed(RazerPacket* pkt);
DllExport uint8_t CallType librazerblade_PacketUtil_getBrightness(RazerPacket* pkt);
DllExport KeyboardRow CallType librazerblade_PacketUtil_getRow(RazerPacket* pkt);
//#endregion

//#region Laptop
DllExport LaptopPtr
CallType librazerblade_Laptop_new(LaptopDescription description, UsbHandle usbHandle, UsbDevice device);
DllExport void CallType librazerblade_Laptop_delete(LaptopPtr self);
DllExport UsbDevice CallType librazerblade_Laptop_getDevice(LaptopPtr self);
DllExport UsbHandle CallType librazerblade_Laptop_getUsbHandle(LaptopPtr self);
DllExport void CallType librazerblade_Laptop_setUsbHandle(LaptopPtr self, UsbHandle v);
DllExport uint8_t CallType librazerblade_Laptop_clampFan(LaptopPtr self, int32_t v);
DllExport BladeCapabilities CallType librazerblade_Laptop_getCapabilities(LaptopPtr self);
DllExport LaptopDescription CallType librazerblade_Laptop_getDescription(LaptopPtr self);
DllExport LaptopState CallType librazerblade_Laptop_getState(LaptopPtr self);
//UCallType nsafe.
DllExport LaptopState* CallType librazerblade_Laptop_getStatePtrUnsafe(LaptopPtr self);
DllExport UsbPacketResult CallType librazerblade_Laptop_queryFanSpeed(LaptopPtr self, int numRetries = 0);
DllExport UsbPacketResult CallType librazerblade_Laptop_queryPowerMode(LaptopPtr self, int numRetries = 0);
DllExport UsbPacketResult CallType librazerblade_Laptop_queryBrightness(LaptopPtr self, int numRetries = 0);
DllExport UsbPacketResult CallType librazerblade_Laptop_queryChromaRow(LaptopPtr self, int rowId, int numRetries = 0);
DllExport LaptopQueryResult CallType librazerblade_Laptop_query(LaptopPtr self, BladeQuery query, int numRetries = 0);
DllExport LaptopQueryResult
CallType librazerblade_Laptop_queryRows(LaptopPtr self, BladeQueryRows rows, int numRetries = 0);
DllExport UsbPacketResult CallType librazerblade_Laptop_setFanSpeed(LaptopPtr self, int32_t speed, int numRetries = 0);
DllExport UsbPacketResult
CallType librazerblade_Laptop_setPowerMode(LaptopPtr self, uint8_t powerMode, int32_t autoFanSpeed, int numRetries = 0);
DllExport UsbPacketResult
CallType librazerblade_Laptop_setBrightness(LaptopPtr self, uint8_t brightness, int numRetries = 0);
DllExport UsbPacketResult CallType librazerblade_Laptop_applyChroma(LaptopPtr self, int numRetries = 0);
DllExport UsbPacketResult
CallType librazerblade_Laptop_sendKeyboardRow(LaptopPtr self, KeyboardRow row, int numRetries = 0);
DllExport UsbPacketResult
CallType librazerblade_Laptop_sendPacketWithRetry(LaptopPtr self, RazerPacket* packet, RazerPacket* output,
                                                  int numRetries = 0,
                                                  int retryIntervalMs = 250);

DllExport int32_t CallType librazerblade_Laptop_getArrayIndex(int32_t q);
//#endregion

//#region Storage
DllExport LaptopDescription
CallType librazerblade_DescriptionStorage_get(uint16_t vendor, uint16_t product, int32_t* idx);
DllExport void CallType librazerblade_DescriptionStorage_put(LaptopDescription description);
DllExport LaptopDescription* CallType librazerblade_DescriptionStorage_getAll(int32_t* size);
DllExport void CallType librazerblade_DescriptionStorage_set(int32_t idx, LaptopDescription description);
DllExport void CallType librazerblade_DescriptionStorage_clear();
//#endregion

//#region Memory
DllExport void* CallType librazerblade_malloc(int32_t size);
DllExport UserData CallType librazerblade_UserData_fromMemory(void *ptr, int32_t size, int32_t autoFree);
DllExport void CallType librazerblade_free(void *ptr);
//#endregion
#ifdef __cplusplus
}
#endif

#endif //LIBRAZERBLADE_LIBRAZERBLADE_H
