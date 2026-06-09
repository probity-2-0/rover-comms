#include <unity.h>

#include "../api/api.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_motor_command_init(void)
{
    MotorCommand_t xCommand;

    xCommand.sLeftSpeed = 100;
    xCommand.sRightSpeed = 100;

    vMotorCommandInit(&xCommand);

    TEST_ASSERT_EQUAL_INT16(
        0,
        xCommand.sLeftSpeed);

    TEST_ASSERT_EQUAL_INT16(
        0,
        xCommand.sRightSpeed);
}

void test_telemetry_init(void)
{
    Telemetry_t xTelemetry;

    vTelemetryInit(&xTelemetry);

    TEST_ASSERT_EQUAL_UINT16(
        0,
        xTelemetry.xSensor.usBattery);

    TEST_ASSERT_EQUAL_INT16(
        0,
        xTelemetry.sLeftMotor);

    TEST_ASSERT_EQUAL_INT16(
        0,
        xTelemetry.sRightMotor);
}

void test_motor_command_assignment(void)
{
    MotorCommand_t xCommand;

    xCommand.sLeftSpeed = 150;
    xCommand.sRightSpeed = 200;

    TEST_ASSERT_EQUAL_INT16(
        150,
        xCommand.sLeftSpeed);

    TEST_ASSERT_EQUAL_INT16(
        200,
        xCommand.sRightSpeed);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(
        test_motor_command_init);

    RUN_TEST(
        test_telemetry_init);

    RUN_TEST(
        test_motor_command_assignment);

    return UNITY_END();
}