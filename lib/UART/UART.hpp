#pragma once

#include <Arduino.h>
#include "../Utils/RingBuffer.hpp"

class UART
{
private:
    HardwareSerial &serial;
    RingBuffer<uint8_t, 256> rxBuffer;

public:
    UART(HardwareSerial &port);

    void begin(uint32_t baud);

    void update();

    bool available();

    bool read(uint8_t &byte);

    void write(uint8_t byte);

    void write(const uint8_t *data, size_t len);
};