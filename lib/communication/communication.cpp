#include "protocol.h"
#include <LoRa_E32.h>
#include <string.h>

#define DESTINATION_ADDL 2

HardwareSerial Serial2(USART2);
LoRa_E32 e32ttl(&Serial2, PA0, PB0, PB10);

// FreeRTOS Queue Handles
static QueueHandle_t xTxQueue = NULL;
static QueueHandle_t xRxQueue = NULL;

/******************************************************************************
 * Internal LoRa Hardware Transmitter
 ******************************************************************************/
static void vE32Send(const Packet_t *pxPacket)
{
    if (pxPacket == NULL)
        return;

    // Fixed transmission to target device (Address: 00 03, Channel: 0x04)
    e32ttl.sendFixedMessage(
        LORA_TARGET_ADDH,
        LORA_TARGET_ADDL,
        LORA_TARGET_CHAN,
        (uint8_t *)pxPacket,
        sizeof(Packet_t));
}

/******************************************************************************
 * Initialize communication subsystem
 ******************************************************************************/
void vCommunicationInit(void)
{
    e32ttl.begin();

#ifdef BUILD_GROUND

    e32ttl.setMode(MODE_0_NORMAL);
#else

    e32ttl.setMode(MODE_2_POWER_SAVING);
#endif
    xTxQueue = xQueueCreate(COMM_QUEUE_LENGTH, sizeof(Packet_t));
    xRxQueue = xQueueCreate(COMM_QUEUE_LENGTH, sizeof(Packet_t));
}

/******************************************************************************
 * Queue packet for transmission
 ******************************************************************************/
BaseType_t xCommunicationSend(const Packet_t *pxPacket)
{
    if (xTxQueue == NULL)
        return pdFAIL;
    return xQueueSend(xTxQueue, pxPacket, 0U);
}

/******************************************************************************
 * Read received packet
 ******************************************************************************/
BaseType_t xCommunicationReceive(Packet_t *pxPacket)
{
    if (xRxQueue == NULL)
        return pdFAIL;
    return xQueueReceive(xRxQueue, pxPacket, 0U);
}

/******************************************************************************
 * Communication gatekeeper task
 ******************************************************************************/
void vCommunicationTask(void *pvParameters)
{
    Packet_t xTxPacket;
    Packet_t xRxPacket;
    (void)pvParameters;

    for (;;)
    {
        // 1. Handle Outbound Messages
        if (xQueueReceive(xTxQueue, &xTxPacket, pdMS_TO_TICKS(5U)) == pdPASS)
        {
            vE32Send(&xTxPacket);
        }

        // 2. Handle Inbound Messages
        if (e32ttl.available() >= (int)sizeof(Packet_t))
        {
            // Use ResponseStructContainer to properly handle raw struct extraction
            ResponseStructContainer rs = e32ttl.receiveMessage(sizeof(Packet_t));

            if (rs.status.code == SUCCESS)
            {
                memcpy(&xRxPacket, rs.data, sizeof(Packet_t));
                xQueueSend(xRxQueue, &xRxPacket, 0U);
            }
            rs.close(); // Clean up memory allocation from library structure
        }

        vTaskDelay(pdMS_TO_TICKS(5U));
    }
}

/******************************************************************************
 * UART ISR pushes received packet
 ******************************************************************************/
void vCommunicationRxIsr(const Packet_t *pxPacket)
{
    if (xRxQueue == NULL)
        return;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(xRxQueue, pxPacket, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}