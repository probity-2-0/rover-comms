#include <string.h>

#include <unity.h>

#include "../ground/ground.c"

/******************************************************************************
 * Mock Communication
 ******************************************************************************/

BaseType_t xCommunicationSend(
    const Packet_t *pxPacket)
{
    (void)pxPacket;
    return pdPASS;
}

BaseType_t xCommunicationReceive(
    Packet_t *pxPacket)
{
    (void)pxPacket;
    return pdFAIL;
}

/******************************************************************************
 * Unity
 ******************************************************************************/

void setUp(void)
{
}

void tearDown(void)
{
}

/******************************************************************************
 * Test Motor Command Creation
 ******************************************************************************/

void test_send_motor_command(void)
{
    vGroundSendMotorCommand(
        100,
        200);

    TEST_PASS();
}

/******************************************************************************
 * Test Telemetry Processing
 ******************************************************************************/

void test_process_telemetry(void)
{
    Packet_t xPacket;
    Telemetry_t xTelemetry;

    memset(
        &xPacket,
        0,
        sizeof(Packet_t));

    memset(
        &xTelemetry,
        0,
        sizeof(Telemetry_t));

    xTelemetry.sLeftMotor = 150;
    xTelemetry.sRightMotor = 175;

    memcpy(
        xPacket.ucPayload,
        &xTelemetry,
        sizeof(Telemetry_t));

    xPacket.ucType =
        PACKET_TELEMETRY;

    vGroundProcessTelemetry(
        &xPacket);

    TEST_ASSERT_EQUAL_INT16(
        150,
        pxGroundGetTelemetry()
            ->sLeftMotor);

    TEST_ASSERT_EQUAL_INT16(
        175,
        pxGroundGetTelemetry()
            ->sRightMotor);
}

/******************************************************************************
 * Main
 ******************************************************************************/

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(
        test_send_motor_command);

    RUN_TEST(
        test_process_telemetry);

    return UNITY_END();
}