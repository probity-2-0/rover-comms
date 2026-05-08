#include "KeyboardCommand.h"

uint8_t KeyboardCommandMapper::map(char key)
{
    switch (key)
    {
    case 'w':
        return CMD_FORWARD;

    case 's':
        return CMD_BACKWARD;

    case 'a':
        return CMD_LEFT;

    case 'd':
        return CMD_RIGHT;

    case ' ':
        return CMD_STOP;

    default:
        return CMD_NONE;
    }
}