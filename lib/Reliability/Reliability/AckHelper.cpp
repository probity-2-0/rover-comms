#include "AckHelper.h"

#include <Arduino.h>

Packet AckHelper::create(uint8_t senderId, uint8_t targetId, uint8_t ackedMsgId, uint8_t msgId)
{
    Packet p;

    p.targetId = targetId;

    p.senderId = senderId;

    p.command = CMD_ACK;

    p.dataA = ackedMsgId;

    p.dataB = 0;

    p.msgId = msgId;

    return p;
}
