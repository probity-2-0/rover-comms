// Detects connection loss
#pragma once

#include <Arduino.h>

class HeartbeatMonitor
{
private:
    uint32_t lastHeartbeat = 0;

public:
    void update();

    void receivedHeartbeat();

    bool connected() const;
};