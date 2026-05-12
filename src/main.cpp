#include <Arduino.h>

#include "../lib/UART/UART.hpp"

#include "../lib/LoRa/LoRa.hpp"

#include "../lib/Reliability/Reliability/ReliableTransport.h"

#include "../lib/Applications/RoverSide/rover.h"
#include "../lib/Applications/GroundSide/ground.h"

#define BUILD_ROVER

UART uart(Serial1);

LoRa lora(uart, PB0);

ReliableTransport reliable(lora);

#ifdef BUILD_ROVER

RoverApplication app(
    lora,
    reliable);

#else

GroundApplication app(
    lora,
    reliable);

#endif

void setup()
{
    Serial.begin(115200);

    lora.begin(9600);

    app.setup();
}

void loop()
{
    app.update();
}