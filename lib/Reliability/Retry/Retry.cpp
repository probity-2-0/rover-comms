#include "Retry.h"

#include "../Reliability/ReliableConfig.hpp"

void RetryManager::start()
{
    timestamp = millis();

    retries = 0;
}

void RetryManager::reset()
{
    retries = 0;

    timestamp = millis();
}

bool RetryManager::timeout() const
{
    return millis() - timestamp > ACK_TIMEOUT_MS;
}

bool RetryManager::retryAllowed() const
{
    return retries < MAX_RETRIES;
}

void RetryManager::increment()
{
    retries++;

    timestamp = millis();
}

uint8_t RetryManager::count() const
{
    return retries;
}