#include "Heartbeat.h"

#include "../Reliability/ReliableConfig.hpp"

void HeartbeatMonitor::update()
{
}

void HeartbeatMonitor::receivedHeartbeat()
{
    lastHeartbeat = millis();
}

bool HeartbeatMonitor::connected() const
{
    return millis() - lastHeartbeat < HEARTBEAT_TIMEOUT_MS;
}