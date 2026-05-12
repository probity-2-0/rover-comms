#include <Arduino.h>

#include "../../lib/Reliability/Retry/Retry.h"

#if defined(__cplusplus)
extern "C"
{
#endif

#include <unity.h>

#if defined(__cplusplus)
}
#endif

void test_retry_exists()
{
    RetryManager r;

    TEST_ASSERT_EQUAL_UINT8(0, r.count());
}

void setup()
{
    delay(1);
    UNITY_BEGIN();
    RUN_TEST(test_retry_exists);
    UNITY_END();
}

void loop() {}
