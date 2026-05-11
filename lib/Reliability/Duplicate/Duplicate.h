// Prevents processing same packets twice
#pragma once

#include <Arduino.h>

class DuplicateDetector
{
private:
    uint8_t lastMsgId = 0xFF;

public:
    bool isDuplicate(uint8_t msgId);
};