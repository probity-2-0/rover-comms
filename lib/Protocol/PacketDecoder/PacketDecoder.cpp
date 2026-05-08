#include "PacketDecoder.h"
#include "../CRC16/CRC16.h"

bool PacketDecoder::input(uint8_t byte,
                          Packet &packet)
{
    if (index == 0)
    {
        if (byte != START_BYTE)
        {
            return false;
        }
    }

    buffer[index++] = byte;

    if (index < 9)
    {
        return false;
    }

    index = 0;

    uint16_t received =
        ((uint16_t)buffer[7] << 8) | buffer[8];

    uint16_t calculated =
        CRC16::calculate(buffer, 7);

    if (received != calculated)
    {
        return false;
    }

    packet.targetId = buffer[1];
    packet.senderId = buffer[2];
    packet.command = buffer[3];

    packet.dataA = buffer[4];
    packet.dataB = buffer[5];

    packet.msgId = buffer[6];

    return true;
}