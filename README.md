# STM32 LoRa Rover Communication System

## Overview

This project implements a bidirectional long-range communication architecture between:

* A Rover Node
* A Ground Station Node

using:

* STM32 Blue Pill (STM32F103C8T6)
* EBYTE E220-400T22D LoRa UART Modules
* UART communication
* PlatformIO
* Arduino framework
* Modern C++ architecture

The system is designed for:

* Long-range telemetry
* Command transmission
* Logging
* Reliability
* Scalability
* Testability
* Maintainability

---

# System Architecture

```text
                   +-------------------+
                   | Ground Station    |
                   | STM32 + E220      |
                   +---------+---------+
                             |
                             |
                       LoRa Wireless
                             |
                             |
                   +---------+---------+
                   | Rover Node        |
                   | STM32 + E220      |
                   +-------------------+
```

# Hardware Stack

## MCU

* STM32F103C8T6 (Blue Pill)

## Radio Module

* EBYTE E220-400T22D
* UART-based LoRa module
* LLCC68 chipset
* 433 MHz band

## Development Environment

* VSCode
* PlatformIO
* Arduino framework
* Unity Test Framework

---

Using UART:

* reduces STM32 CPU load
* simplifies firmware
* improves reliability
* avoids SPI timing issues
* improves long-range communication stability

The STM32 behaves as a high-level serial controller instead of directly managing the LoRa PHY layer.

---

# Hardware Connections

## E220 UART Wiring

| E220 Pin | STM32 Pin  |
| -------- | ---------- |
| TXD      | RX         |
| RXD      | TX         |
| AUX      | GPIO Input |
| M0       | GND        |
| M1       | GND        |
| VCC      | 3.3V / 5V  |
| GND      | GND        |

---

## STM32 Blue Pill UART Pins

| UART    | TX  | RX   |
| ------- | --- | ---- |
| Serial1 | PA9 | PA10 |

---

# Software Architecture
| Module        | Responsibility             |
| ------------- | -------------------------- |
| communication | Send/receive packets       |
| api           | Shared data structures     |
| ground        | Ground station behavior    |
| rover         | Rover behavior             |
| state         | RX/TX/SLEEP state tracking |
| main          | Task creation and startup  |
