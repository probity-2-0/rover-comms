#pragma once

#include "../Utils/RingBuffer.hpp"

template <typename T, size_t Size>
using Queue = RingBuffer<T, Size>;