#include <Arduino.h>
#include <unity.h>

#include "../state/rover_state.c"

void setUp(void)
{
}

void tearDown(void)
{
}

/******************************************************************************
 * Initial State
 ******************************************************************************/

void test_initial_state(void)
{
    TEST_ASSERT_EQUAL(
        STATE_RX,
        xStateGet());
}

/******************************************************************************
 * RX -> PROCESS
 ******************************************************************************/

void test_packet_received(void)
{
    vStateSet(
        STATE_RX);

    vStatePacketReceived();

    TEST_ASSERT_EQUAL(
        STATE_PROCESS,
        xStateGet());
}

/******************************************************************************
 * PROCESS -> TX
 ******************************************************************************/

void test_packet_processed(void)
{
    vStateSet(
        STATE_PROCESS);

    vStatePacketProcessed();

    TEST_ASSERT_EQUAL(
        STATE_TX,
        xStateGet());
}

/******************************************************************************
 * TX -> RX
 ******************************************************************************/

void test_packet_sent(void)
{
    vStateSet(
        STATE_TX);

    vStatePacketSent();

    TEST_ASSERT_EQUAL(
        STATE_RX,
        xStateGet());
}

/******************************************************************************
 * RX -> SLEEP
 ******************************************************************************/

void test_sleep_transition(void)
{
    vStateSet(
        STATE_RX);

    vStateIdle();

    TEST_ASSERT_EQUAL(
        STATE_SLEEP,
        xStateGet());
}

/******************************************************************************
 * SLEEP -> RX
 ******************************************************************************/

void test_wake_transition(void)
{
    vStateSet(
        STATE_SLEEP);

    vStateWake();

    TEST_ASSERT_EQUAL(
        STATE_RX,
        xStateGet());
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(
        test_initial_state);

    RUN_TEST(
        test_packet_received);

    RUN_TEST(
        test_packet_processed);

    RUN_TEST(
        test_packet_sent);

    RUN_TEST(
        test_sleep_transition);

    RUN_TEST(
        test_wake_transition);

    return UNITY_END();
}