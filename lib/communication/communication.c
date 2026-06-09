#include "protocol.h"

#include <Arduino.h>
#include <STM32FreeRTOS.h>
/*
 * Replace these with actual E32 UART functions.
 */
static void vE32Send(
    const Packet_t *pxPacket)
{
    (void)pxPacket;
}

static QueueHandle_t xTxQueue = NULL;
static QueueHandle_t xRxQueue = NULL;

/******************************************************************************
 * Initialize communication subsystem
 ******************************************************************************/
void vCommunicationInit(void)
{
    xTxQueue =
        xQueueCreate(
            COMM_QUEUE_LENGTH,
            sizeof(Packet_t));

    xRxQueue =
        xQueueCreate(
            COMM_QUEUE_LENGTH,
            sizeof(Packet_t));
}

/******************************************************************************
 * Queue packet for transmission
 ******************************************************************************/
BaseType_t xCommunicationSend(
    const Packet_t *pxPacket)
{
    return xQueueSend(
        xTxQueue,
        pxPacket,
        0U);
}

/******************************************************************************
 * Read received packet
 ******************************************************************************/
BaseType_t xCommunicationReceive(
    Packet_t *pxPacket)
{
    return xQueueReceive(
        xRxQueue,
        pxPacket,
        0U);
}

/******************************************************************************
 * Communication gatekeeper task
 ******************************************************************************/
void vCommunicationTask(
    void *pvParameters)
{
    Packet_t xPacket;

    (void)pvParameters;

    for (;;)
    {
        if (xQueueReceive(
                xTxQueue,
                &xPacket,
                pdMS_TO_TICKS(10U)) == pdPASS)
        {
            vE32Send(&xPacket);
        }
    }
}

/******************************************************************************
 * UART ISR pushes received packet
 ******************************************************************************/
void vCommunicationRxIsr(
    const Packet_t *pxPacket)
{
    BaseType_t xHigherPriorityTaskWoken =
        pdFALSE;

    xQueueSendFromISR(
        xRxQueue,
        pxPacket,
        &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(
        xHigherPriorityTaskWoken);
}