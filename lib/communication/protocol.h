#include <stdint.h>
#include <Arduino.h>

#define COMM_QUEUE_LENGTH 10U
#define COMM_PAYLOAD_SIZE 32U

typedef enum
{
    PACKET_MOTOR = 0,
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

BaseType_t xCommunicationSend(
    const Packet_t *pxPacket);

BaseType_t xCommunicationReceive(
    Packet_t *pxPacket);

void vCommunicationTask(
    void *pvParameters);

void vCommunicationRxIsr(
    const Packet_t *pxPacket);
