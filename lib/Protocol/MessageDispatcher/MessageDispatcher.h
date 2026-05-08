#pragma once
// Routes commands to handlers
#include "../COmmands/Packet.h"

class MessageDispatcher
{
public:
    void dispatch(const Packet &packet);
};