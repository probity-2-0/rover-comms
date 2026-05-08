#pragma once

#include <Arduino.h>
// Defines all command IDS used by rover and ground station
enum CommandType : uint8_t
{
    CMD_NONE = 0x00,

    CMD_FORWARD = 0x01,
    CMD_BACKWARD = 0x02,
    CMD_LEFT = 0x03,
    CMD_RIGHT = 0x04,
    CMD_STOP = 0x05,

    CMD_HEARTBEAT = 0x10,
    CMD_TELEMETRY = 0x11,

    CMD_ACK = 0x20,
    CMD_LOG = 0x30
};