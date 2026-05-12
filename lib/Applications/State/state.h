#pragma once

class State
{
public:
    enum Mode
    {
        BOOT,
        IDLE,
        ACTIVE,
        FAILSAFE
    };

private:
    Mode current = BOOT;

public:
    void set(Mode mode);

    Mode get() const;

    bool is(Mode mode) const;
};