#pragma once

#include <Arduino.h>

class RetryManager
{
private:
    uint32_t timestamp = 0;

    uint8_t retries = 0;

public:
    void start();

    void reset();

    bool timeout() const;

    bool retryAllowed() const;

    void increment();

    uint8_t count() const;
};