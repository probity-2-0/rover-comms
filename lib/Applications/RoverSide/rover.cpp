#include "rover.h"

#include "../../Protocol/Commands/Packet.h"
#include "../../Protocol/Commands/CommandTypes.h"

#include "../../Reliability/Reliability/ReliableTransport.h"

#include <Arduino.h>

RoverApplication::RoverApplication(
    LoRa &radio,
    ReliableTransport &transport)
    : lora(radio),
      reliable(transport)
{
}

void RoverApplication::setup()
{
    state.set(State::IDLE);

    Serial.println("ROVER READY");
}

void RoverApplication::update()
{
    reliable.process();

    processIncoming();

    processSlaveSensors();

    processTelemetry();

    processFailsafe();
}

void RoverApplication::processIncoming()
{
    while (lora.available())
    {
        uint8_t byte;

        if (lora.read(byte))
        {
            Packet packet;

            if (decoder.input(byte,
                              packet))
            {
                if (packet.command == CMD_ACK)
                {
                    reliable.handleAck(
                        packet.dataA);

                    return;
                }

                // send an ACK for the received packet (senderId 0x01 is this rover)
                reliable.sendAck(
                    0x01,
                    packet.senderId,
                    packet.msgId);

                heartbeat.receivedHeartbeat();

                issueMotorCommand(
                    packet.command);
            }
        }
    }
}

void RoverApplication::issueMotorCommand(
    uint8_t command)
{
    switch (command)
    {
    case CMD_FORWARD:
        Serial.println("MOTOR FWD");
        break;

    case CMD_BACKWARD:
        Serial.println("MOTOR BACK");
        break;

    case CMD_LEFT:
        Serial.println("MOTOR LEFT");
        break;

    case CMD_RIGHT:
        Serial.println("MOTOR RIGHT");
        break;

    case CMD_STOP:
        Serial.println("MOTOR STOP");
        break;
    }

    // send command to slave STM32 here
}

void RoverApplication::processSlaveSensors()
{
    // receive:
    // battery
    // imu
    // lidar
    // gps
    // etc

    // from slave STM32s
}

void RoverApplication::processTelemetry()
{
    static uint32_t lastSend = 0;

    if (millis() - lastSend < 1000)
    {
        return;
    }

    lastSend = millis();

    sendTelemetry(
        12,
        80);
}

void RoverApplication::sendTelemetry(
    uint8_t battery,
    uint8_t lidar)
{
    if (reliable.busy())
    {
        return;
    }

    Packet packet;

    packet.targetId = 0xFE;

    packet.senderId = 0x01;

    packet.command = CMD_TELEMETRY;

    packet.dataA = battery;

    packet.dataB = lidar;

    packet.msgId =
        telemetryMsgId++;

    reliable.send(packet);
}

void RoverApplication::processFailsafe()
{
    heartbeat.update();

    if (!heartbeat.connected())
    {
        state.set(State::FAILSAFE);

        issueMotorCommand(
            CMD_STOP);

        Serial.println("FAILSAFE");
    }
}