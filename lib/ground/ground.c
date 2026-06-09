#include "FreeRTOS.h"
#include "task.h"

#include "../communication/protocol.h"
#include "../api/api.h"

/******************************************************************************
 * Ground Data
 ******************************************************************************/

static Telemetry_t xTelemetry;

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
        if (xCommunicationReceive(
                &xPacket) == pdPASS)
        {
            if (xPacket.ucType == PACKET_TELEMETRY)
            {
                vGroundProcessTelemetry(
                    &xPacket);
            }
        }

        vTaskDelay(
            pdMS_TO_TICKS(10U));
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