#pragma once

template <typename T, size_t Size>
class RingBuffer
{
private:
    T buffer[Size];

    volatile size_t head = 0;
    volatile size_t tail = 0;

public:
    bool push(const T &item)
    {
        size_t next = (head + 1) % Size;

        if (next == tail)
            return false;

        buffer[head] = item;
        head = next;
        return true;
    }
    bool pop(T &item)
    {
        if (head == tail)
            return false;

        item = buffer[tail];
        tail = (tail + 1) % Size;
        return true;
    }
    bool empty() const
    {
        return head == tail;
    }
};