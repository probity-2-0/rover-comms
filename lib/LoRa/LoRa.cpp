#include "LoRa.hpp"

LoRa::LoRa(UART &uartRef, uint8_t aux)
    : uart(uartRef),
      auxPin(aux)
{
}

void LoRa::begin(uint32_t baud)
{
    pinMode(auxPin, INPUT);
    uart.begin(baud);
}

bool LoRa::ready()
{
    return digitalRead(auxPin);
}

void LoRa::update()
{
    uart.update();
}
bool LoRa::available()
{
    return uart.available();
}
bool LoRa::read(uint8_t &byte)
{
    return uart.read(byte);
}
void LoRa::send(const uint8_t *data, size_t len)
{
    while (!ready())
        ;
    uart.write(data, len);
}