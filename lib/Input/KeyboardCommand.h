#pragma once
// Maps keyboard keys to rover commands.
#include "../Protocol/Commands/CommandTypes.h"

class KeyboardCommandMapper
{
public:
    static uint8_t map(char key);
};