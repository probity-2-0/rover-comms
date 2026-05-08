#include <Arduino.h>
#include <unity.h>

#include "../../lib/Protocol/Commands/Packet.h"
#include "../../lib/Protocol/PacketEncoder/PacketEncoder.h"
#include "../../lib/Protocol//PacketDecoder/PacketDecoder.h"

void test_packet_decode()
{
    Packet tx =
        {
            1,
            2,
            3,
            4,
            5,
            6};

    uint8_t bytes[9];

    PacketEncoder::encode(tx,
                          bytes);

    Packet rx;

    PacketDecoder decoder;

    bool success = false;

    for (int i = 0; i < 9; i++)
    {
        success =
            decoder.input(bytes[i],
                          rx);
    }

    TEST_ASSERT_TRUE(success);

    TEST_ASSERT_EQUAL(
        tx.command,
        rx.command);
}

void setup()
{
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(test_packet_decode);

    UNITY_END();
}

void loop()
{
}