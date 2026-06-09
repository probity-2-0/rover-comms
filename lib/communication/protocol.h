#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <Arduino.h>
#include <STM32FreeRTOS.h>

#define COMM_QUEUE_LENGTH 10U
#define COMM_PAYLOAD_SIZE 32U
/******************************************************************************
 * Physical EBYTE LoRa E32 Hardware Pin Assignments
 ******************************************************************************/
#define LORA_AX_PIN PA0  // AUX Pin (Monitors internal module state changes)
#define LORA_M0_PIN PB0  // Mode Control Line 0
#define LORA_M1_PIN PB10 // Mode Control Line 1

// Point-To-Point fixed routing parameters
#define LORA_TARGET_ADDH 0x00
#define LORA_TARGET_ADDL 0x03
#define LORA_TARGET_CHAN 0x04

/******************************************************************************
 * Typedef Protocols
 ******************************************************************************/

typedef enum
{
    PACKET_INPUT = 0,
    PACKET_MOTOR,
    PACKET_SENSOR,
    PACKET_TELEMETRY
} PacketType_t;

typedef struct
{
    uint8_t ucType;
    uint8_t ucPayload[COMM_PAYLOAD_SIZE];
} Packet_t;

/* Public API */
void vCommunicationInit(void);

BaseType_t xCommunicationSend(const Packet_t *pxPacket);
BaseType_t xCommunicationReceive(Packet_t *pxPacket);
void vCommunicationTask(void *pvParameters);
void vCommunicationRxIsr(const Packet_t *pxPacket);

#endif // PROTOCOL_H