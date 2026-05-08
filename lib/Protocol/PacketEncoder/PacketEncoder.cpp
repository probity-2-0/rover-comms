// Stream parser with CRC validation.
#include "PacketEncoder.h"
#include "CRC16/CRC16.h"

void PacketEncode::encode(const Packet &packet,
                          uint8_t *buffer)
{
    buffer[0] = START_BYTE;

    buffer[1] = packet.targetId;
    buffer[2] = packet.senderId;
    buffer[3] = packet.command;

    buffer[4] = packet.dataA;
    buffer[5] = packet.dataB;

    buffer[6] = packet.msgId;

    uint16_t crc =
        CRC16::calculate(buffer, 7);

    buffer[7] = (crc >> 8) & 0xFF;
    buffer[8] = crc & 0xFF;
}