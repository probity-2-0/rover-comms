#pragma once

#include <Arduino.h>
// Logical packet structure without CRC.
struct Packet
{
    uint8_t targetId;
    uint8_t senderId;
    uint8_t command;
    uint8_t dataA;
    uint8_t dataB;
    uint8_t msgId;
};