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

---

# Communication Goals

The system supports:

* Rover telemetry
* Remote control commands
* Logging
* Heartbeat monitoring
* ACK/retry reliability
* Failsafe triggering

---

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

The project uses:

* Composition-first design
* Interface-driven architecture
* State machines
* Queue-based communication
* Layered services
* Dependency inversion

Deep inheritance hierarchies are intentionally avoided.

---

# High-Level Software Layers

```text
+------------------------------------------------+
|                APPLICATION LAYER               |
|------------------------------------------------|
| RoverController                                |
| GroundStationController                        |
| MissionManager                                 |
| FailSafeManager                                |
+------------------------------------------------+

+------------------------------------------------+
|                 SERVICE LAYER                  |
|------------------------------------------------|
| TelemetryService                               |
| CommandService                                 |
| LoggingService                                 |
| HeartbeatService                               |
+------------------------------------------------+

+------------------------------------------------+
|                PROTOCOL LAYER                  |
|------------------------------------------------|
| PacketEncoder                                  |
| PacketDecoder                                  |
| CRC16                                          |
| Dispatcher                                     |
+------------------------------------------------+

+------------------------------------------------+
|                TRANSPORT LAYER                 |
|------------------------------------------------|
| LoRaDriver                                     |
| UARTDriver                                     |
+------------------------------------------------+

+------------------------------------------------+
|                HARDWARE LAYER                  |
|------------------------------------------------|
| Motors                                         |
| Sensors                                        |
| GPIO                                           |
| Timers                                         |
+------------------------------------------------+
```

---



# Packet Structure

The protocol uses compact fixed-size binary packets.

```text
+------------+------------+------------+------------+
| Target ID  | Sender ID  | Command    | Data A     |
+------------+------------+------------+------------+
| Data B     | Msg ID     | CRC16                  |
+------------+------------+-------------------------+
```

---

# Packet Definition

```cpp
struct Packet
{
    uint8_t targetId;
    uint8_t senderId;
    uint8_t command;
    uint8_t dataA;
    uint8_t dataB;
    uint8_t msgId;
    uint16_t crc;
};
```

---

# Reliability Features

The communication layer includes:

* CRC16 validation
* ACK packets
* retry logic
* heartbeat monitoring
* message IDs
* duplicate detection

---

# Rover State Machine

```text
             +---------+
             | INIT    |
             +----+----+
                  |
                  v
          +-------+-------+
          | READY         |
          +-------+-------+
                  |
                  v
          +-------+-------+
          | ACTIVE        |
          +-------+-------+
                  |
             signal loss
                  |
                  v
          +-------+-------+
          | FAILSAFE      |
          +-------+-------+
```

---

# Communication State Machine

```text
            +-------------+
            | WAIT RX     |
            +------+------+
                   |
            packet received
                   |
                   v
            +------+------+
            | VALIDATE    |
            +------+------+
                   |
                   v
            +------+------+
            | PROCESS     |
            +------+------+
                   |
                   v
            +------+------+
            | SEND ACK    |
            +------+------+
                   |
                   v
               WAIT RX
```

---

# Telemetry Flow

```text
Sensors
   ↓
TelemetryService
   ↓
PacketEncoder
   ↓
TX Queue
   ↓
LoRa Driver
   ↓
Wireless Link
```

---

# RX Flow

```text
UART ISR
   ↓
RingBuffer
   ↓
PacketDecoder
   ↓
MessageDispatcher
   ↓
CommandProcessor
```

---

# Communication Rules

## Ground Station

Acts as:

```text
MASTER NODE
```

Responsibilities:

* send commands
* receive telemetry
* monitor heartbeat
* display logs

---

## Rover

Acts as:

```text
SLAVE NODE
```

Responsibilities:

* execute commands
* send telemetry
* send ACKs
* trigger failsafe

---

# AUX Pin Handling

The AUX pin is critical.

## AUX HIGH

```text
Module Ready
```

## AUX LOW

```text
Module Busy
```

Firmware must never transmit while AUX is LOW.
---

# Future Enhancements

Potential future additions:

* DMA UART
* FreeRTOS
* encryption
* OTA updates
* adaptive telemetry
* packet prioritization
* persistent configuration
* GPS waypoint navigation
* autonomous modes

---

# Design Principles

The architecture prioritizes:

* simplicity
* modularity
* determinism
* testability
* debuggability
* reliability
* queue-based communication
* robust testing
