#include "state.h"

void State::set(Mode mode)
{
    current = mode;
}

State::Mode State::get() const
{
    return current;
}

bool State::is(Mode mode) const
{
    return current == mode;
}