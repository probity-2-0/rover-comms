#include "task.h"
#include <Arduino.h>
#include <STM32FreeRTOS.h>

#include "../communication/protocol.h"
#include "../api/api.h"
#include "../state/rover_state.c"

/******************************************************************************
 * Ground Data
 ******************************************************************************/

static Telemetry_t xTelemetry;
static InputCommand_t xInputCommand;
static MotorCommand_t xLastCommand;

/******************************************************************************
 * Process Input Command
 ******************************************************************************/

void vGroundProcessInputCommand(
    const Packet_t *pxPacket)
{
    memcpy(
        &xInputCommand,
        pxPacket->ucPayload,
        sizeof(InputCommand_t));

    vStatePacketReceived();
}

/******************************************************************************
 * Convert Input to Motor Command
 ******************************************************************************/

static MotorCommand_t vGroundConvertInputToMotor(
    const InputCommand_t *pxInput)
{
    MotorCommand_t xCommand;
    vMotorCommandInit(&xCommand);

    int16_t sForward = 0;
    int16_t sRotate = 0;

    if (pxInput->ucW)
        sForward = 100;
    if (pxInput->ucS)
        sForward = -100;

    if (pxInput->ucA)
        sRotate = -50;
    if (pxInput->ucD)
        sRotate = 50;

    xCommand.sLeftSpeed = sForward + sRotate;
    xCommand.sRightSpeed = sForward - sRotate;

    return xCommand;
}

/******************************************************************************
 * Send Motor Command
 ******************************************************************************/

void vGroundSendMotorCommand(
    int16_t sLeftSpeed,
    int16_t sRightSpeed)
{
    Packet_t xPacket;
    MotorCommand_t xCommand;

    xCommand.sLeftSpeed = sLeftSpeed;
    xCommand.sRightSpeed = sRightSpeed;

    xPacket.ucType = PACKET_MOTOR;

    memcpy(
        xPacket.ucPayload,
        &xCommand,
        sizeof(MotorCommand_t));

    xCommunicationSend(
        &xPacket);

    xLastCommand = xCommand;
    vStatePacketProcessed();
    vStatePacketSent();
}

/******************************************************************************
 * Process Incoming Telemetry
 ******************************************************************************/

void vGroundProcessTelemetry(
    const Packet_t *pxPacket)
{
    memcpy(
        &xTelemetry,
        pxPacket->ucPayload,
        sizeof(Telemetry_t));

    Serial.println("--- NEW TELEMETRY PACKET RECEIVED ---");
    Serial.print("Battery Voltage: ");
    Serial.println(xTelemetry.xSensor.usBattery);
    Serial.print("IMU Accel X: ");
    Serial.println(xTelemetry.xSensor.sAccelX);
}

/******************************************************************************
 * Ground Task
 ******************************************************************************/

void vGroundTask(
    void *pvParameters)
{
    Packet_t xPacket;

    (void)pvParameters;

    for (;;)
    {
        if (xCommunicationReceive(&xPacket) == pdPASS)
        {
            if (xPacket.ucType == PACKET_INPUT)
            {
                vGroundProcessInputCommand(&xPacket);

                MotorCommand_t xCommand =
                    vGroundConvertInputToMotor(&xInputCommand);

                vGroundSendMotorCommand(
                    xCommand.sLeftSpeed,
                    xCommand.sRightSpeed);
            }
            else if (xPacket.ucType == PACKET_TELEMETRY)
            {
                vGroundProcessTelemetry(&xPacket);
                vStatePacketReceived();
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10U));
    }
}

/******************************************************************************
 * Get Latest Telemetry
 ******************************************************************************/

const Telemetry_t *pxGroundGetTelemetry(
    void)
{
    return &xTelemetry;
}

/******************************************************************************
 * Get Latest Input Command
 ******************************************************************************/

const InputCommand_t *pxGroundGetInputCommand(
    void)
{
    return &xInputCommand;
}

/******************************************************************************
 * Get Last Motor Command Sent
 ******************************************************************************/

const MotorCommand_t *pxGroundGetLastCommand(
    void)
{
    return &xLastCommand;
}