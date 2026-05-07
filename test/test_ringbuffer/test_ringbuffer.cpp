#include <Arduino.h>
#include <unity.h>

#include "../../lib/Utils/RingBuffer.hpp"

RingBuffer<int, 4> buffer;

void setUp()
{
}

void tearDown()
{
}

void test_push_success()
{
    TEST_ASSERT_TRUE(buffer.push(1));
    TEST_ASSERT_TRUE(buffer.push(2));
}

void test_pop_success()
{
    int value;
    buffer.push(42);
    TEST_ASSERT_TRUE(buffer.pop(value));
    TEST_ASSERT_EQUAL(42, value);
}
void test_empty_buffer()
{
    int value;
    TEST_ASSERT_FALSE(buffer.pop(value));
}
void test_buffer_full()
{
    TEST_ASSERT_TRUE(buffer.push(1));
    TEST_ASSERT_TRUE(buffer.push(2));
    TEST_ASSERT_TRUE(buffer.push(3));

    TEST_ASSERT_FALSE(buffer.push(4));
}

void setup()
{
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(test_push_success);
    RUN_TEST(test_pop_success);
    RUN_TEST(test_empty_buffer);
    RUN_TEST(test_buffer_full);

    UNITY_END();
}
void loop()
{
}