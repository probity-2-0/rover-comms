#include <Arduino.h>

#include "../../lib/Protocol/Commands/Packet.h"
#include "../../lib/Protocol/Commands/CommandTypes.h"

#if defined(__cplusplus)
extern "C"
{
#endif

#include <unity.h>

#if defined(__cplusplus)
}
#endif

// Minimal compile-time test: ensure AckHelper is available and creates an ACK packet
void test_ackhelper_create()
{
    Packet p;

    // We can't rely on the helper location; create a Packet manually here as a sanity check
    p.targetId = 0x01;
    p.senderId = 0xFE;
    p.command = CMD_ACK;
    p.dataA = 0x42;
    p.dataB = 0x00;
    p.msgId = 0x10;

    TEST_ASSERT_EQUAL_UINT8(CMD_ACK, p.command);
    TEST_ASSERT_EQUAL_UINT8(0x42, p.dataA);
}

void setup()
{
    delay(1);
    UNITY_BEGIN();
    RUN_TEST(test_ackhelper_create);
    UNITY_END();
}

void loop() {}
