// Rover-side orchestration
#pragma once

#include "../../LoRa/LoRa.hpp"

#include "../../Protocol/PacketDecoder/PacketDecoder.h"
#include "../../Protocol/MessageDispatcher/MessageDispatcher.h"

#include "../../Reliability/Reliability/ReliableTransport.h"
#include "../../Reliability/Heartbeat/Heartbeat.h"
#include "../../Reliability/Duplicate/Duplicate.h"

#include "../State/state.h"

class RoverApplication
{
private:
    LoRa &lora;

    ReliableTransport &reliable;

    PacketDecoder decoder;

    HeartbeatMonitor heartbeat;

    State state;

    uint8_t telemetryMsgId = 0;

public:
    RoverApplication(
        LoRa &radio,
        ReliableTransport &transport);

    void setup();

    void update();

private:
    void processIncoming();

    void processSlaveSensors();

    void processTelemetry();

    void processFailsafe();

    void issueMotorCommand(
        uint8_t command);

    void sendTelemetry(
        uint8_t battery,
        uint8_t lidar);
};