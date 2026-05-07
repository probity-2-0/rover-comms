#pragma once

#include <Arduino.h>
#include "../UART/UART.hpp"

class LoRa
{
private:
    UART &uart;
    uint8_t auxPin;

public:
    LoRa(UART &uart, uint8_t auxPin);

    void begin(uint32_t baud);

    bool ready();

    void update();

    bool available();

    bool read(uint8_t &byte);

    void send(const uint8_t *data, size_t len);
};