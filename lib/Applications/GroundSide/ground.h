// Rover-side orchestration
#pragma once

#include "../../LoRa/LoRa.hpp"

#include "../../Protocol/PacketDecoder/PacketDecoder.h"
#include "../../Protocol/MessageDispatcher/MessageDispatcher.h"

#include "../../Reliability/Reliability/ReliableTransport.h"
#include "../../Reliability/Heartbeat/Heartbeat.h"
#include "../../Reliability/Duplicate/Duplicate.h"

#include "../State/state.h"

class GroundApplication
{
private:
    LoRa &lora;

    ReliableTransport &reliable;

    PacketDecoder decoder;

    HeartbeatMonitor heartbeat;

    State state;

    uint8_t msgId = 0;

public:
    GroundApplication(
        LoRa &radio,
        ReliableTransport &transport);

    void setup();

    void update();

private:
    void processInput();

    void processIncoming();

    void processFailsafe();

    void sendCommand(uint8_t cmd);
};