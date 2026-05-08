#include "MessageDispatcher.h"
#include "../Commands/CommandTypes.h"

#include <Arduino.h>

void MessageDispatcher::dispatch(
    const Packet &packet)
{
    switch (packet.command)
    {
    case CMD_FORWARD:
        Serial.println("MOVE FORWARD");
        break;

    case CMD_BACKWARD:
        Serial.println("MOVE BACKWARD");
        break;

    case CMD_LEFT:
        Serial.println("TURN LEFT");
        break;

    case CMD_RIGHT:
        Serial.println("TURN RIGHT");
        break;

    case CMD_STOP:
        Serial.println("STOP");
        break;

    case CMD_HEARTBEAT:
        Serial.println("HEARTBEAT");
        break;

    default:
        Serial.println("UNKNOWN");
        break;
    }
}