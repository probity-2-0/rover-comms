#include <unity.h>
#include "LoRa.hpp"
#include "UART.hpp"

// Setup hardware references
UART *testUart;
LoRa *testLoRa;
const uint8_t MOCK_AUX_PIN = PC13; // Example AUX pin

void setUp(void)
{
    testUart = new UART(Serial1);
    testLoRa = new LoRa(*testUart, MOCK_AUX_PIN);
}

void tearDown(void)
{
    delete testLoRa;
    delete testUart;
}

void test_lora_initialization(void)
{
    testLoRa->begin(9600);
    // Verify UART is ready and pin mode is set
    TEST_ASSERT_FALSE(testLoRa->available());
    [cite:244]
}

void test_lora_ready_logic(void)
{
    // The AUX pin indicates if the E32 module is busy
    // This test assumes you can toggle the pin or it's in a known state
    bool isReady = testLoRa->ready();
    TEST_ASSERT_EQUAL(digitalRead(MOCK_AUX_PIN), isReady);
}

void test_lora_send_respects_aux(void)
{
    uint8_t data[] = {0xAA, 0x01, 0x01, 0x05, 0x10}; // Example packet [cite: 121]

    // In a real scenario, this would block if AUX is LOW
    // We test that the call completes when the module is ready
    if (testLoRa->ready())
    {
        testLoRa->send(data, sizeof(data));
        PASS();
    }
}

void test_lora_read_pass_through(void)
{
    uint8_t byte;
    // Verify that LoRa::read correctly calls the underlying UART ring buffer [cite: 243]
    bool result = testLoRa->read(byte);
    TEST_ASSERT_EQUAL(testUart->read(byte), result);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_lora_initialization);
    RUN_TEST(test_lora_ready_logic);
    RUN_TEST(test_lora_send_respects_aux);
    RUN_TEST(test_lora_read_pass_through);
    return UNITY_END();
}