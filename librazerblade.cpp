//
// Created by alex on 08/01/2022.
//

#ifdef LIBUSB_DEFAULT_IMPLEMENTATION

#include "libusb/libusb.h"
#include "DescriptionStorage.h"

//TODO: Implement other compilers here
#pragma comment(lib, __FILE__"\\..\\libusb\\msvc\\libusb-1.0.lib")
#endif

#include "librazerblade.h"
#include "Usb/UsbInterface.h"
#include "Packets/PacketUtil.h"
#include "Packets/PacketFactory.h"
#include "Laptop.h"

using namespace librazerblade;

#ifdef LIBUSB_DEFAULT_IMPLEMENTATION
static bool librazerblade_libusbinitialized = false;

static int32_t libusb_controlTransferFunction(void* handle, uint32_t requestType, uint32_t request, uint32_t value,
                                              uint32_t reportIndex, uint8_t* buffer, uint16_t bufferSize,
                                              uint32_t timeout,
                                              void* aux)
{
    return libusb_control_transfer((libusb_device_handle*) handle, requestType, request, value, reportIndex, buffer,
                                   bufferSize, timeout);
}

static int32_t libusb_sendControlMessage(void* handle, uint8_t* buffer, uint16_t size, uint32_t timeout,
                                         uint32_t delayAfter, void* aux)
{
    uint32_t request = 0x09;//HID_REQ_SET_REPORT; // 0x09
    uint32_t request_type = 0x21;//USB_TYPE_CLASS | USB_RECIP_INTERFACE | USB_DIR_OUT; // 0x21
    uint32_t value = 0x300;
    uint32_t report_index = 0x02;

    int len = libusb_control_transfer((libusb_device_handle*) handle, request_type, request, value, report_index,
                                      buffer, size,
                                      timeout);

    Utils::usleep(delayAfter);

    //if (len != size)
    //    printf("Razer laptop control: Device data transfer failed.");

    return ((len < 0) ? len : ((len != size) ? -EIO : 0));
}

static void* libusb_openDevice(UsbDevice* dev)
{
    libusb_device_handle* handle = nullptr;
    auto v = libusb_open((libusb_device*) dev->device, &handle);
    if (v != 0)
        return nullptr;

    return handle;
}

static void libusb_closeHandle(void* handle, UsbDevice* dev)
{
    if (handle == nullptr)
        return;

    libusb_close((libusb_device_handle*) handle);
}

static UsbDeviceList* libusb_listDevices()
{
    auto* list = new UsbDeviceList();
    memset(list, 0, sizeof(UsbDeviceList));

    libusb_device** devs = nullptr;
    auto numDevices = libusb_get_device_list(nullptr, &devs);
    list->userData = devs;

    if (numDevices == 0) {
        return list;
    }

    if (numDevices < 0) {
        list->error = numDevices;
        return list;
    }

    auto sz = sizeof(UsbDevice) * numDevices;
    list->devices = (UsbDevice*) (malloc(sz));
    memset(list->devices, 0, sz);
    list->count = 0;

    int i = 0;
    libusb_device* dev;
    while (list->count < numDevices && ((dev = devs[i++]) != nullptr)) {
        libusb_device_descriptor desc = {};
        int r = libusb_get_device_descriptor(dev, &desc);

        if (r == 0) {
            UsbDevice output = {dev, nullptr, desc.idVendor, desc.idProduct};
            list->devices[list->count++] = output;
        }
    }

    return list;
}

static void libusb_freeDeviceList(UsbDeviceList* list)
{
    if (list == nullptr)
        return;

    auto listPtr = (libusb_device**) list->userData;

    if (listPtr != nullptr)
        libusb_free_device_list(listPtr, 1);

    if (list->devices != nullptr)
        free(list->devices);

    delete list;
}

static int32_t initializeLibUsb()
{
    if (librazerblade_libusbinitialized)
        return 0;

    auto ret = libusb_init(nullptr);
    if (ret >= 0) {
        librazerblade_libusbinitialized = true;
    }

    UsbInterfaceImplementation impl{};
    impl.controlTransfer = libusb_controlTransferFunction;
    impl.sendControlMessage = libusb_sendControlMessage;
    impl.openDevice = libusb_openDevice;
    impl.closeHandle = libusb_closeHandle;
    impl.freeDeviceList = libusb_freeDeviceList;
    impl.listDevices = libusb_listDevices;

    gUsb.setImplementation(impl);
    return ret;
}

static void closeLibUsb()
{
    if (!librazerblade_libusbinitialized)
        return;

    librazerblade_libusbinitialized = false;
    libusb_exit(nullptr);
}

#endif

extern "C" {
//#region Usb Interface

DllExport int32_t CallType librazerblade_initialize()
{
#ifdef WITH_DEFAULT_DEVICES
    librazerblade::gStorage.initializeWithDefault();
#endif

#ifdef LIBUSB_DEFAULT_IMPLEMENTATION
    return initializeLibUsb();
#else
    return -1;
#endif
}

DllExport void CallType librazerblade_deinitialize()
{
#ifdef LIBUSB_DEFAULT_IMPLEMENTATION
    closeLibUsb();
#endif
}


DllExport void CallType librazerblade_UsbInterface_closeHandle(void* handle, UsbDevice* device)
{
    gUsb.closeHandle(handle, device);
}

DllExport void* CallType librazerblade_UsbInterface_openDevice(UsbDevice* device)
{
    return gUsb.openDevice(device);
}

DllExport UsbDeviceList* CallType librazerblade_UsbInterface_listDevices()
{
    return gUsb.listDevices();
}

DllExport void CallType librazerblade_UsbInterface_freeDeviceList(UsbDeviceList* list)
{
    gUsb.freeDeviceList(list);
}

DllExport void
CallType librazerblade_UsbInterface_setImplementation(UsbInterfaceImplementation anImplementation)
{
    gUsb.setImplementation(anImplementation);
}

DllExport UsbInterfaceImplementation CallType librazerblade_UsbInterface_getImplementation()
{
    return gUsb.getImplementation();
}

DllExport int32_t CallType
librazerblade_UsbInterface_controlTransfer(void* handle, uint32_t requestType, uint32_t request, uint32_t value,
                                           uint32_t reportIndex,
                                           uint8_t* buffer, uint16_t bufferSize, uint32_t timeout, void* aux)
{
    return gUsb.controlTransfer(handle, requestType, request, value, reportIndex, buffer, bufferSize,
                                timeout, aux);
}

DllExport RazerPacket
CallType librazerblade_UsbInterface_sendPayload(void* handle, RazerPacket* request, uint32_t delayAfter = 1000,
                                                UsbPacketResult* result = nullptr, void* aux = nullptr)
{
    return gUsb.sendPayload(handle, request, delayAfter, result, aux);
}

DllExport int32_t CallType
librazerblade_UsbInterface_getUsbResponse(void* handle, RazerPacket* request, RazerPacket* responseBuffer,
                                          unsigned long maxWait = 1000, void* aux = nullptr)
{
    return gUsb.getUsbResponse(handle, request, responseBuffer, maxWait, aux);
}
//#endregion

//#region Packet Factory
DllExport uint8_t CallType librazerblade_PacketFactory_getDataSize(BladePacketType type)
{
    return PacketFactory::getDataSize(type);
}

DllExport RazerPacket CallType librazerblade_PacketFactory_empty()
{
    return PacketFactory::empty();
}

DllExport RazerPacket CallType
librazerblade_PacketFactory_createRazerPacket(uint8_t command_class, BladePacketType type,
                                              BladePacketDirection direction,
                                              uint8_t size)
{
    return PacketFactory::createRazerPacket(command_class, type, direction, size);
}

DllExport RazerPacket
CallType librazerblade_PacketFactory_fan(uint8_t fanSpeedDiv100, BladePacketDirection direction)
{
    return PacketFactory::fan(fanSpeedDiv100, direction);
}

DllExport RazerPacket
CallType librazerblade_PacketFactory_power(uint8_t powerMode, int32_t autoFanSpeed, BladePacketDirection direction)
{
    return PacketFactory::power(powerMode, autoFanSpeed > 0, direction);
}

DllExport RazerPacket CallType librazerblade_PacketFactory_row(KeyboardRow row, BladePacketDirection direction)
{
    return PacketFactory::row(row, direction);
}

DllExport RazerPacket CallType librazerblade_PacketFactory_applyChroma()
{
    return PacketFactory::applyChroma();
}

DllExport RazerPacket
CallType librazerblade_PacketFactory_brightness(uint8_t brightness, BladePacketDirection direction)
{
    return PacketFactory::brightness(brightness, direction);
}
//#endregion

//#region Packet Util
DllExport uint8_t CallType librazerblade_PacketUtil_getFanValueRaw(RazerPacket* pkt)
{
    return PacketUtil::getFanValueRaw(pkt);
}

DllExport int32_t CallType librazerblade_PacketUtil_getFanValue(RazerPacket* pkt)
{
    return PacketUtil::getFanValue(pkt);
}

DllExport uint8_t CallType librazerblade_PacketUtil_getManualFanSpeed(RazerPacket* pkt)
{
    return PacketUtil::getManualFanSpeed(pkt);
}

DllExport uint8_t CallType librazerblade_PacketUtil_getBrightness(RazerPacket* pkt)
{
    return PacketUtil::getBrightness(pkt);
}

DllExport KeyboardRow CallType librazerblade_PacketUtil_getRow(RazerPacket* pkt)
{
    return PacketUtil::getRow(pkt);
}
//#endregion

//#region Laptop
DllExport LaptopPtr CallType librazerblade_Laptop_new(LaptopDescription description, UsbHandle usbHandle, UsbDevice device)
{
    return new Laptop(description, usbHandle, device);
}

DllExport void CallType librazerblade_Laptop_delete(LaptopPtr self)
{
    auto instance = (Laptop*) self;
    delete instance;
}

DllExport UsbDevice CallType librazerblade_Laptop_getDevice(LaptopPtr self)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->getDevice();
}

DllExport UsbHandle CallType librazerblade_Laptop_getUsbHandle(LaptopPtr self)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->getUsbHandle();
}

DllExport void CallType librazerblade_Laptop_setUsbHandle(LaptopPtr self, UsbHandle v)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return;

    instance->setUsbHandle(v);
}

DllExport uint8_t CallType librazerblade_Laptop_clampFan(LaptopPtr self, int32_t v)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->clampFan(v);
}

DllExport BladeCapabilities CallType librazerblade_Laptop_getCapabilities(LaptopPtr self)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->getCapabilities();
}

DllExport LaptopDescription CallType librazerblade_Laptop_getDescription(LaptopPtr self)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->getDescription();
}

DllExport LaptopState CallType librazerblade_Laptop_getState(LaptopPtr self)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->getState();
}

//Unsafe.
DllExport LaptopState* CallType librazerblade_Laptop_getStatePtrUnsafe(LaptopPtr self)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->getStatePtrUnsafe();
}

DllExport UsbPacketResult CallType librazerblade_Laptop_queryFanSpeed(LaptopPtr self, int numRetries)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->queryFanSpeed(numRetries);
}

DllExport UsbPacketResult CallType librazerblade_Laptop_queryPowerMode(LaptopPtr self, int numRetries)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->queryPowerMode(numRetries);
}

DllExport UsbPacketResult CallType librazerblade_Laptop_queryBrightness(LaptopPtr self, int numRetries)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->queryBrightness(numRetries);
}

DllExport UsbPacketResult CallType librazerblade_Laptop_queryChromaRow(LaptopPtr self, int rowId, int numRetries)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->queryChromaRow(rowId, numRetries);
}

DllExport LaptopQueryResult CallType librazerblade_Laptop_query(LaptopPtr self, BladeQuery query, int numRetries)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->query(query, numRetries);
}

DllExport LaptopQueryResult CallType librazerblade_Laptop_queryRows(LaptopPtr self, BladeQueryRows rows, int numRetries)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->queryRows(rows, numRetries);
}

DllExport UsbPacketResult CallType librazerblade_Laptop_setFanSpeed(LaptopPtr self, int32_t speed, int numRetries)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->setFanSpeed(speed, numRetries);
}

DllExport UsbPacketResult
CallType librazerblade_Laptop_setPowerMode(LaptopPtr self, uint8_t powerMode, int32_t autoFanSpeed, int numRetries)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->setPowerMode(powerMode, autoFanSpeed > 0, numRetries);
}

DllExport UsbPacketResult
CallType librazerblade_Laptop_setBrightness(LaptopPtr self, uint8_t brightness, int numRetries)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->setBrightness(brightness, numRetries);
}

DllExport UsbPacketResult CallType librazerblade_Laptop_applyChroma(LaptopPtr self, int numRetries)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->applyChroma(numRetries);
}

DllExport UsbPacketResult CallType librazerblade_Laptop_sendKeyboardRow(LaptopPtr self, KeyboardRow row, int numRetries)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->sendKeyboardRow(row, numRetries);
}

DllExport UsbPacketResult CallType
librazerblade_Laptop_sendPacketWithRetry(LaptopPtr self, RazerPacket* packet, RazerPacket* output, int numRetries,
                                         int retryIntervalMs)
{
    auto instance = (Laptop*) self;
    if (instance == nullptr)
        return {};

    return instance->sendPacketWithRetry(packet, output, numRetries, retryIntervalMs);
}

DllExport int32_t CallType librazerblade_Laptop_getArrayIndex(int32_t q)
{
    return Laptop::getArrayIndex(q);
}
//#endregion

//#region Storage
DllExport LaptopDescription
CallType librazerblade_DescriptionStorage_get(uint16_t vendor, uint16_t product, int32_t* idx)
{
    return gStorage.get(vendor, product, idx);
}
DllExport void CallType librazerblade_DescriptionStorage_put(LaptopDescription description)
{
    return gStorage.put(description);
}
DllExport LaptopDescription* CallType librazerblade_DescriptionStorage_getAll(int32_t* size)
{
    return gStorage.getAll(size);
}
DllExport void CallType librazerblade_DescriptionStorage_set(int32_t idx, LaptopDescription description)
{
    return gStorage.set(idx, description);
}
//#endregion
}

