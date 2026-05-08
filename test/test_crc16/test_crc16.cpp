#include <Arduino.h>
#include <unity.h>

#include "../../lib/Protocol/CRC16/CRC16.h"

void test_crc16()
{
    uint8_t data[3] =
        {
            1, 2, 3};

    uint16_t crc =
        CRC16::calculate(data, 3);

    TEST_ASSERT_NOT_EQUAL(0, crc);
}

void setup()
{
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(test_crc16);

    UNITY_END();
}

void loop()
{
}