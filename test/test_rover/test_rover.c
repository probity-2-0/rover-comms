#include <string.h>

#include <unity.h>

#include "../rover/rover.c"

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
 * Test Motor Command Processing
 ******************************************************************************/

void test_motor_command_processing(void)
{
    Packet_t xPacket;
    MotorCommand_t xCommand;

    memset(
        &xPacket,
        0,
        sizeof(Packet_t));

    xCommand.sLeftSpeed = 120;
    xCommand.sRightSpeed = 150;

    memcpy(
        xPacket.ucPayload,
        &xCommand,
        sizeof(MotorCommand_t));

    xPacket.ucType = PACKET_MOTOR;

    vRoverProcessMotorCommand(
        &xPacket);

    TEST_ASSERT_EQUAL_INT16(
        120,
        pxRoverGetMotorCommand()
            ->sLeftSpeed);

    TEST_ASSERT_EQUAL_INT16(
        150,
        pxRoverGetMotorCommand()
            ->sRightSpeed);
}

/******************************************************************************
 * Test Telemetry Update
 ******************************************************************************/

void test_telemetry_update(void)
{
    Telemetry_t xTelemetry;

    memset(
        &xTelemetry,
        0,
        sizeof(Telemetry_t));

    xTelemetry.sLeftMotor = 100;
    xTelemetry.sRightMotor = 110;

    xTelemetry.xSensor.usBattery = 95;

    vRoverUpdateTelemetry(
        &xTelemetry);

    TEST_ASSERT_EQUAL_INT16(
        100,
        pxRoverGetTelemetry()
            ->sLeftMotor);

    TEST_ASSERT_EQUAL_INT16(
        110,
        pxRoverGetTelemetry()
            ->sRightMotor);

    TEST_ASSERT_EQUAL_UINT16(
        95,
        pxRoverGetTelemetry()
            ->xSensor.usBattery);
}

/******************************************************************************
 * Test Telemetry Packet Creation
 ******************************************************************************/

void test_send_telemetry(void)
{
    vRoverSendTelemetry();

    TEST_PASS();
}

/******************************************************************************
 * Main
 ******************************************************************************/

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(
        test_motor_command_processing);

    RUN_TEST(
        test_telemetry_update);

    RUN_TEST(
        test_send_telemetry);

    return UNITY_END();
}