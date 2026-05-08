#include <unity.h>
#include "UART.hpp"

UART *testUart;

void setUp(void)
{

    testUart = new UART(Serial1);
}

void tearDown(void)
{
    delete testUart;
}

void test_uart_initial_state(void)
{

    TEST_ASSERT_FALSE(testUart->available())
}

void test_uart_write_single_byte(void)
{

    testUart->write(0xAA);
}

void test_uart_buffer_integration(void)
{
    uint8_t receivedByte = 0;

    testUart->update();

    if (testUart->available())
    {
        bool success = testUart->read(receivedByte);
        TEST_ASSERT_TRUE(success);
    }
}

void test_packet_start_byte_logic(void)
{

    testUart->write(0xAA);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_uart_initial_state);
    RUN_TEST(test_uart_write_single_byte);
    RUN_TEST(test_uart_buffer_integration);
    RUN_TEST(test_packet_start_byte_logic);
    return UNITY_END();
}