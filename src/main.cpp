#include "FreeRTOS.h"
#include "task.h"

#include "../communication/protocol.h"
#include "../state/rover_state.c"

extern void vCommunicationTask(
    void *pvParameters);

#ifdef BUILD_GROUND

extern void vGroundTask(
    void *pvParameters);

#else

extern void vRoverTask(
    void *pvParameters);

#endif

int main(void)
{
    vCommunicationInit();

    xTaskCreate(
        vCommunicationTask,
        "COMM",
        256,
        NULL,
        2,
        NULL);

#ifdef BUILD_GROUND

    vStateSet(STATE_RX);

    xTaskCreate(
        vGroundTask,
        "GROUND",
        256,
        NULL,
        1,
        NULL);

#else

    vStateSet(STATE_RX);

    xTaskCreate(
        vRoverTask,
        "ROVER",
        256,
        NULL,
        1,
        NULL);

#endif

    vTaskStartScheduler();

    while (1)
    {
    }
}