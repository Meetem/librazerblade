//
// Created by alex on 08/01/2022.
//

#ifndef LIBRAZERBLADE_BLADESTRUCTS_H
#define LIBRAZERBLADE_BLADESTRUCTS_H

#include "Core.h"
#include "BladeDeviceId.h"

#define RAZER_USB_REPORT_LEN 90
#define DEFAULT_TIMEOUT 5000

enum BladeQuery
{
    QueryNone = 0,
    QueryFanSpeed = 0b00000001,
    QueryPowerMode = 0b00000010,
    QueryBrightness = 0b00000100,
    QueryAll = 0x7FFFFFFF,
    QueryCount = 3,
};

enum BladeQueryRows
{
    QueryRow0 = 0b00000001,
    QueryRow1 = 0b00000010,
    QueryRow2 = 0b00000100,
    QueryRow3 = 0b00001000,
    QueryRow4 = 0b00010000,
    QueryRow5 = 0b00100000,
    QueryRowCount = 6,
};

enum BladeResponse
{
    RazerResponseNone = 0x00,
    RazerResponseBusy = 0x01,
    RazerResponseSuccess = 0x02,
    RazerResponseFailure = 0x03,
    RazerResponseTimeout = 0x04,
    RazerResponseNotSupported = 0x05,

    RazerResponseDoesntMatch = 0xFF
};

enum BladePacketDirection
{
    Set = 0x00,
    Get = 0x01,
    HostToDevice = Set,
    DeviceToHost = Get
};

enum BladePacketType
{
    PktFan = 0x01,
    PktPower = 0x02,
    PktGetBrightness = 0x03,
    PktSetBrightness = 0x04,
    PktChromaApply = 0x0a,
    PktChromaSetRow = 0x0b,
};

typedef struct UsbHandle_s{
    int32_t autoRelease;
    void *handle;
}UsbHandle;

typedef struct KeyboardRow_s
{
    uint8_t rowid; // Row ID (0-5) used by the EC to update each row
    RGB24 keys[15]; // 15 keys per row (regardless if the LEDs are present or not)
} KeyboardRow;

// Keyboard structs
typedef struct KeyboardInfo_s
{
    uint8_t brightness; // 0-255 brightness
    KeyboardRow rows[6]; // 6 rows
} KeyboardInfo;

typedef union TransactionId_s
{
    uint8_t id;
    struct transaction_parts
    {
        uint8_t device: 3;
        uint8_t id: 5;
    } parts;
} TransactionId;

typedef union CommandId_s
{
    uint8_t id;
    struct command_id_parts
    {
        uint8_t id: 7;
        uint8_t direction: 1;
    } parts;
} CommandId;

/* Status:
* 0x00 New Command
* 0x01 Command Busy
* 0x02 Command Successful
* 0x03 Command Failure
* 0x04 Command No Response / Command Timeout
* 0x05 Command Not Support
*
* Transaction ID used to group request-response, device useful when multiple devices are on one usb
* Remaining Packets is the number of remaining packets in the sequence
* Protocol Type is always 0x00
* Data Size is the size of payload, cannot be greater than 80. 90 = header (8B) + data + CRC (1B) + Reserved (1B)
* Command Class is the type of command being issued
* Command ID is the type of command being send. Direction 0 is Host->Device, Direction 1 is Device->Host. AKA Get LED 0x80, Set LED 0x00
*
* */
typedef struct RazerPacket_s
{
    uint8_t status;
    TransactionId transaction_id;
    uint16_t remaining_packets;
    uint8_t protocol_type; // 0x00
    uint8_t data_size;
    uint8_t command_class;
    CommandId command_id;
    uint8_t args[80];
    uint8_t crc;
    uint8_t reserved; // 0x00
} RazerPacket;

#endif //LIBRAZERBLADE_BLADESTRUCTS_H
