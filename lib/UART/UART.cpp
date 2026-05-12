#include "UART.hpp"

UART::UART(HardwareSerial &port)
    : serial(port)
{
}

void UART::begin(uint32_t baud)
{
    serial.begin(baud);
}
void UART::update()
{
    while (serial.available())
    {
        rxBuffer.push(serial.read());
    }
}

bool UART::available()
{
    return !rxBuffer.empty();
}

bool UART::read(uint8_t &byte)
{
    return rxBuffer.pop(byte);
}
void UART::write(uint8_t byte)
{
    serial.write(byte);
}
void UART::write(const uint8_t *data, size_t len)
{
    serial.write(data, len);
}