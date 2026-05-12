#include "ground.h"

#include "../../Protocol/Commands/Packet.h"
#include "../../Protocol/Commands/CommandTypes.h"

#include <Arduino.h>

GroundApplication::GroundApplication(
    LoRa &radio,
    ReliableTransport &transport)
    : lora(radio),
      reliable(transport)
{
}

void GroundApplication::setup()
{
    state.set(State::IDLE);

    Serial.println("GROUND READY");
}

void GroundApplication::update()
{
    reliable.process();

    processInput();

    processIncoming();

    processFailsafe();
}

void GroundApplication::processInput()
{
    if (!Serial.available())
    {
        return;
    }

    char key = Serial.read();

    switch (key)
    {
    case 'w':
        sendCommand(CMD_FORWARD);
        break;

    case 's':
        sendCommand(CMD_BACKWARD);
        break;

    case 'a':
        sendCommand(CMD_LEFT);
        break;

    case 'd':
        sendCommand(CMD_RIGHT);
        break;

    case ' ':
        sendCommand(CMD_STOP);
        break;
    }
}

void GroundApplication::sendCommand(
    uint8_t cmd)
{
    if (reliable.busy())
    {
        return;
    }

    Packet packet;

    packet.targetId = 0x01;

    packet.senderId = 0xFE;

    packet.command = cmd;

    packet.dataA = 0;

    packet.dataB = 0;

    packet.msgId = msgId++;

    reliable.send(packet);
}

void GroundApplication::processIncoming()
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
                }

                if (packet.command ==
                    CMD_TELEMETRY)
                {
                    Serial.print("BAT:");

                    Serial.println(
                        packet.dataA);

                    Serial.print("LIDAR:");

                    Serial.println(
                        packet.dataB);
                }

                if (packet.command ==
                    CMD_HEARTBEAT)
                {
                    heartbeat.receivedHeartbeat();
                }
            }
        }
    }
}

void GroundApplication::processFailsafe()
{
    heartbeat.update();

    if (!heartbeat.connected())
    {
        state.set(State::FAILSAFE);

        Serial.println("LINK LOST");
    }
}