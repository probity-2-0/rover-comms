#pragma once

#include "../Commands/Packet.h"

class PacketEncode
{
public:
    static constexpr uint8_t START_BYTE = 0xAA;

    static constexpr size_t PACKET_SIZE = 9;

    static void encode(const Packet &packet, uint8_t *buffer);
};