#pragma once

#include <Arduino.h>

class CRC16
{
public:
    static uint16_t calculate(const uint8_t *data, size_t length);
};