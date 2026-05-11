#include "Duplicate.h"

bool DuplicateDetector::isDuplicate(
    uint8_t msgId)
{
    if (msgId == lastMsgId)
    {
        return true;
    }

    lastMsgId = msgId;

    return false;
}