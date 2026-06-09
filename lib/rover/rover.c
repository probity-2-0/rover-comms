#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "../communication/protocol.h"
#include "../api/api.h"
#include "../state/rover_state.c"

/******************************************************************************
 * Rover Data
 ******************************************************************************/

static MotorCommand_t xMotorCommand;
static Telemetry_t xTelemetry;
static uint32_t ulTelemetrySendCounter = 0;

/******************************************************************************
 * Process Motor Command
 ******************************************************************************/

void vRoverProcessMotorCommand(
    const Packet_t *pxPacket)
{
    memcpy(
        &xMotorCommand,
        pxPacket->ucPayload,
        sizeof(MotorCommand_t));

    vStatePacketReceived();
}

/******************************************************************************
 * Update Telemetry
 ******************************************************************************/

void vRoverUpdateTelemetry(
    const Telemetry_t *pxTelemetry)
{
    memcpy(
        &xTelemetry,
        pxTelemetry,
        sizeof(Telemetry_t));
}

/******************************************************************************
 * Send Telemetry
 ******************************************************************************/

void vRoverSendTelemetry(void)
{
    Packet_t xPacket;

    memset(
        &xPacket,
        0,
        sizeof(Packet_t));

    xPacket.ucType = PACKET_TELEMETRY;

    memcpy(
        xPacket.ucPayload,
        &xTelemetry,
        sizeof(Telemetry_t));

    xCommunicationSend(
        &xPacket);

    vStatePacketProcessed();
    vStatePacketSent();
}

/******************************************************************************
 * Rover Task
 ******************************************************************************/

void vRoverTask(
    void *pvParameters)
{
    Packet_t xPacket;

    (void)pvParameters;

    vTelemetryInit(&xTelemetry);

    for (;;)
    {
        if (xCommunicationReceive(
                &xPacket) == pdPASS)
        {
            if (xPacket.ucType == PACKET_MOTOR)
            {
                vRoverProcessMotorCommand(
                    &xPacket);

                vStatePacketProcessed();
            }
        }

        ulTelemetrySendCounter++;
        if (ulTelemetrySendCounter >= 10)
        {
            xTelemetry.sLeftMotor = xMotorCommand.sLeftSpeed;
            xTelemetry.sRightMotor = xMotorCommand.sRightSpeed;

            vRoverSendTelemetry();
            ulTelemetrySendCounter = 0;
            vStatePacketReceived();
        }

        vTaskDelay(
            pdMS_TO_TICKS(10U));
    }
}

/******************************************************************************
 * Accessors
 ******************************************************************************/

const MotorCommand_t *pxRoverGetMotorCommand(
    void)
{
    return &xMotorCommand;
}

const Telemetry_t *pxRoverGetTelemetry(
    void)
{
    return &xTelemetry;
}