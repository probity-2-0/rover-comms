#pragma once

#include "../Commands/Packet.h"

class PacketDecoder
{
private:
    static constexpr uint8_t START_BYTE = 0xAA;

    uint8_t buffer[9];

    size_t index = 0;

public:
    bool input(uint8_t byte,
               Packet &packet);
};