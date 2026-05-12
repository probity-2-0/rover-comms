// Handles Retires and ACK tracking
#include "ReliableTransport.h"

#include "ReliableConfig.hpp"
#include "AckHelper.h"

ReliableTransport::ReliableTransport(
    LoRa &loraRef)
    : lora(loraRef)
{
}

void ReliableTransport::send(Packet &packet)
{
    PacketEncode::encode(packet,
                         txBuffer);

    lora.send(txBuffer, 9);

    pending.packet = packet;

    pending.timestamp = millis();

    pending.retries = 0;

    pending.awaitingAck = true;
}

void ReliableTransport::process()
{
    if (!pending.awaitingAck)
    {
        return;
    }

    uint32_t now = millis();

    if (now - pending.timestamp <
        ACK_TIMEOUT_MS)
    {
        return;
    }

    if (pending.retries >= MAX_RETRIES)
    {
        Serial.println("MAX RETRIES");

        pending.awaitingAck = false;

        return;
    }

    PacketEncode::encode(
        pending.packet,
        txBuffer);

    lora.send(txBuffer, 9);

    pending.timestamp = now;

    pending.retries++;

    Serial.println("RETRY SENT");
}

void ReliableTransport::handleAck(
    uint8_t msgId)
{
    if (!pending.awaitingAck)
    {
        return;
    }

    if (pending.packet.msgId != msgId)
    {
        return;
    }

    pending.awaitingAck = false;

    Serial.println("ACK RECEIVED");
}

bool ReliableTransport::busy() const
{
    return pending.awaitingAck;
}

void ReliableTransport::handleAck(const Packet &packet)
{
    // packet.dataA is expected to contain the acked message id
    handleAck(packet.dataA);
}

void ReliableTransport::sendAck(uint8_t senderId, uint8_t targetId, uint8_t ackedMsgId)
{
    Packet ack = AckHelper::create(senderId, targetId, ackedMsgId);

    // ACKs are control packets and should not themselves be tracked for ACK
    uint8_t buf[9];

    PacketEncode::encode(ack, buf);

    lora.send(buf, 9);
}