#include <Arduino.h>
#include <unity.h>

#include "../../lib/Protocol/Commands/Packet.h"
#include "../../lib/Protocol/PacketEncoder/PacketEncoder.h"

void test_packet_encode()
{
    Packet packet =
        {
            1,
            2,
            3,
            4,
            5,
            6};

    uint8_t buffer[9];

    PacketEncoder::encode(packet,
                          buffer);

    TEST_ASSERT_EQUAL_HEX8(
        0xAA,
        buffer[0]);
}

void setup()
{
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(test_packet_encode);

    UNITY_END();
}

void loop()
{
}