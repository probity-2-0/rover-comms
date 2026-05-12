#pragma once

#include "../../Protocol/Commands/Packet.h"
#include "../../Protocol/Commands/CommandTypes.h"

// Helper to create simple ACK Packets used by ReliableTransport and apps.
struct AckHelper
{
    // Build an ACK packet with acked message id in dataA
    static Packet create(uint8_t senderId, uint8_t targetId, uint8_t ackedMsgId, uint8_t msgId = 0);
};
