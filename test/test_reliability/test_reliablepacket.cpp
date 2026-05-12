#include <Arduino.h>

#include "../../lib/Reliability/Reliability/ReliablePacket.h"

#if defined(__cplusplus)
extern "C"
{
#endif

#include <unity.h>

#if defined(__cplusplus)
}
#endif

void test_reliablepacket_defaults()
{
    ReliablePacket rp;

    TEST_ASSERT_FALSE(rp.awaitingAck);
    TEST_ASSERT_EQUAL_UINT8(0, rp.retries);
    TEST_ASSERT_EQUAL_UINT32(0, rp.timestamp);
}

void setup()
{
    delay(1);
    UNITY_BEGIN();
    RUN_TEST(test_reliablepacket_defaults);
    UNITY_END();
}

void loop() {}
