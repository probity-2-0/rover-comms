// Central reliability manager
#pragma once

#include "../../Protocol/Commands/Packet.h"
#include "../../Protocol/PacketEncoder/PacketEncoder.h"
#include "../../LoRa/LoRa.hpp"

#include "ReliablePacket.h"

class ReliableTransport
{
private:
    LoRa &lora;

    ReliablePacket pending;

    uint8_t txBuffer[9];

public:
    ReliableTransport(LoRa &loraRef);

    void send(Packet &packet);

    void process();

    void handleAck(uint8_t msgId);

    bool busy() const;
};