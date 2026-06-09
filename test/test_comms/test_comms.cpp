#include <unity.h>

#include "../communication/protocol.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_packet_creation(void)
{
    Packet_t xPacket;

    xPacket.ucType = PACKET_MOTOR;

    TEST_ASSERT_EQUAL_UINT8(
        PACKET_MOTOR,
        xPacket.ucType);
}

void test_communication_init(void)
{
    vCommunicationInit();

    TEST_PASS();
}

void test_send_receive(void)
{
    Packet_t xTxPacket;
    Packet_t xRxPacket;

    vCommunicationInit();

    xTxPacket.ucType = PACKET_SENSOR;

    TEST_ASSERT_EQUAL(
        pdPASS,
        xCommunicationSend(
            &xTxPacket));

    vCommunicationRxIsr(
        &xTxPacket);

    TEST_ASSERT_EQUAL(
        pdPASS,
        xCommunicationReceive(
            &xRxPacket));

    TEST_ASSERT_EQUAL_UINT8(
        PACKET_SENSOR,
        xRxPacket.ucType);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(
        test_packet_creation);

    RUN_TEST(
        test_communication_init);

    RUN_TEST(
        test_send_receive);

    return UNITY_END();
}