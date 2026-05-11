// Tracks packet delivery metadata
#pragma once
#include "../../Protocol/Commands/Packet.h"

struct ReliablePacket
{
    Packet packet;

    uint32_t timestamp = 0;

    uint8_t retries = 0;

    bool awaitingAck = false;
};