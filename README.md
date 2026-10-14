# S32K144 CAN-Based Reverse Parking Assist ECU

**Automotive Embedded Systems | Embedded C | FlexCAN | CANdb++ | TSMaster**

## 1. Project Overview

This project implements a CAN-based reverse parking assistance ECU prototype using the **NXP S32K144 microcontroller** and Embedded C.

The firmware receives gear-status and distance information over CAN, processes the received data, and controls GPIO outputs according to predefined parking-warning conditions.

A DBC database is created using **CANdb++** to define CAN messages, while **TSMaster** is used to simulate and monitor CAN communication for functional testing.

The project demonstrates an automotive embedded development workflow involving ECU firmware development, CAN database design, communication simulation, and functional test planning.

## 2. Project Features

- Embedded C firmware development for NXP S32K144.
- CAN communication using the FlexCAN peripheral.
- CAN database creation using CANdb++.
- CAN message simulation and monitoring using TSMaster.
- Gear-status detection through received CAN messages.
- Distance-dependent parking warning logic.
- GPIO control using S32 SDK driver APIs.
- Functional test scenarios for different gear and distance conditions.

## 3. Hardware and Software Requirements

| Component | Technology |
|---|---|
| Microcontroller | NXP S32K144 |
| Development environment | S32 Design Studio |
| Programming language | Embedded C |
| CAN peripheral | FlexCAN |
| CAN database | CANdb++ |
| CAN simulation | TSMaster |
| GPIO driver | S32 SDK PINS driver |
| Timing API | OSIF |

## 4. System Architecture

```text
Gear Status (CAN ID 0x100)
              |
              |
Distance (CAN ID 0x101)
              |
              v
          CAN Bus
              |
              v
     NXP S32K144 ECU
              |
              v
      FlexCAN Reception
              |
              v
      CAN Data Processing
              |
              v
     Parking Control Logic
              |
       +------+------+
       |             |
       v             v
Reverse Indicator  Warning Indicator
```

## 5. CAN Communication Configuration

The firmware receives two standard CAN messages.

| CAN ID | Signal Purpose | Data Byte | Payload Length |
|---|---|---|---|
| `0x100` | Gear status | Byte 0 | 1 byte |
| `0x101` | Distance information | Byte 0 | 1 byte |

The firmware uses two FlexCAN receive mailboxes:

- Mailbox 0 receives CAN ID `0x100`.
- Mailbox 1 receives CAN ID `0x101`.

The received message data is processed by the `get_data()` function.

**Gear encoding:** The firmware assumes that the value `2` represents reverse gear. This encoding must match the transmitter configuration and the DBC definition.

The current implementation reads the first data byte directly rather than using an automatically generated DBC signal decoder.

## 6. CAN Database Development Using CANdb++

A DBC database was created using CANdb++ to define the messages used by the parking assistance system.

The database documents the CAN message identifiers and associated signal definitions, providing a structured reference for communication between the transmitting node and the S32K144 ECU.

### CAN Message Definitions

- `0x100` — Gear status
- `0x101` — Distance information

### CANdb++ Screenshot

<img width="1919" height="1143" alt="image" src="https://github.com/user-attachments/assets/264f1b5c-5c2f-4a13-a4ea-c2905e2f9ee9" />

*Insert your actual CANdb++ screenshot showing message identifiers, signal definitions, and message properties.*

## 7. CAN Simulation Using TSMaster

TSMaster is used to simulate CAN traffic and monitor the messages associated with the parking assistance logic.

The simulation scenarios cover different gear values and distance inputs to evaluate the expected behavior of the firmware.

### TSMaster Configuration

<img width="1916" height="1133" alt="image" src="https://github.com/user-attachments/assets/1c2aa8c4-db00-429b-b35e-b5c647413d71" />


*Insert your actual TSMaster configuration screenshot.*

## 8. Embedded Firmware Implementation

The firmware is developed in Embedded C using S32 SDK peripheral drivers.

### Peripheral Initialization

The application initializes the clock, pin configuration, and FlexCAN peripheral using the following APIs:

- `CLOCK_DRV_Init()`
- `PINS_DRV_Init()`
- `FLEXCAN_DRV_Init()`
- `FLEXCAN_DRV_ConfigRxMb()`

### Data Reception

The `get_data()` function processes received CAN messages.

- For CAN ID `0x100`, the first payload byte updates the gear variable.
- For CAN ID `0x101`, the first payload byte updates the distance variable.

### Parking Control Logic

The `parking_control()` function evaluates the gear and distance values.

When reverse gear is selected (`gear == 2`), the firmware applies the following logic:

| Distance | Implemented Behavior |
|---|---|
| Greater than 100 | Sets the warning output HIGH |
| 51–100 | Toggles the warning output with a 1000 ms delay |
| 0–50 | Toggles the warning output with a 500 ms delay |

When reverse gear is not selected, both configured output pins are set HIGH.

The physical indicator state depends on the GPIO pin configuration and whether the outputs are active-HIGH or active-LOW.

### Firmware Screenshot

<img width="1919" height="1141" alt="image" src="https://github.com/user-attachments/assets/8d06d6a9-65b4-4e47-9c37-bcb12a51d178" />

<img width="1919" height="1136" alt="image" src="https://github.com/user-attachments/assets/b48f87f1-be81-44ea-86a5-b1da3b0f40a1" />

<img width="1919" height="1143" alt="image" src="https://github.com/user-attachments/assets/b1e04c3f-0f94-4914-bcde-23ca068bb0b3" />

*Insert your actual S32 Design Studio screenshot showing the CAN reception and parking-control implementation.*

## 9. Functional Test Scenarios

The following test cases are derived from the current firmware logic.

| Test Case | Gear Value | Distance | Expected Behavior |
|---|---:|---:|---|
| Reverse gear, far distance | 2 | 120 | Warning output set HIGH |
| Reverse gear, medium distance | 2 | 75 | Warning output toggles with a 1000 ms delay |
| Reverse gear, close distance | 2 | 30 | Warning output toggles with a 500 ms delay |
| Non-reverse gear | Not 2 | 75 | Both output pins set HIGH |

### Example CAN Payloads

| CAN ID | Example Payload | Purpose |
|---|---|---|
| `0x100` | `02` | Reverse gear |
| `0x101` | `78` | Distance = 120 |
| `0x101` | `4B` | Distance = 75 |
| `0x101` | `1E` | Distance = 30 |

These examples assume that the values are transmitted in the first data byte.

**Note:** These are expected test scenarios, not a claim that every test has passed. Record the actual observed results during simulation or hardware testing.

## 10. Output Validation

The output validation stage involves checking the behavior of the configured GPIO outputs under different gear and distance conditions.

![Youtube Demo Video](Screenshots/06_Output_Testing.png)

*Insert a genuine screenshot or photograph of your tested outputs, board indicators, or other relevant test evidence.*

## 11. Current Limitations

- CAN reception uses blocking driver calls.
- Warning timing uses blocking delays.
- CAN message timeout detection is not implemented.
- Stale or missing CAN data is not explicitly handled.
- Received gear and distance values are not comprehensively validated.
- The firmware does not currently decode signals through a generated DBC decoder.

## 12. Future Improvements

- Implement non-blocking CAN reception or interrupt-driven processing.
- Use a periodic timer for warning control.
- Add CAN timeout detection and communication fault handling.
- Validate incoming gear and distance values.
- Define safe output behavior for communication failures.
- Add automated test cases and boundary-value testing.
- Improve DBC-based signal decoding and documentation.
- Expand testing with repeatable simulation and hardware validation.

## 13. Skills Demonstrated

- Embedded C Programming
- NXP S32K144 Microcontroller
- FlexCAN Peripheral Configuration
- Automotive CAN Communication
- CANdb++ and DBC Database Development
- TSMaster CAN Simulation and Monitoring
- GPIO Control
- S32 Design Studio
- S32 SDK Peripheral Drivers
- ECU Functional Testing

## 14. Project Structure

```text
S32K144-CAN-Based-Reverse-Parking-Assist-ECU/
|
+-- Parking_System_project/
|   +-- board/
|   +-- doc/
|   +-- Project_Settings/
|   +-- SDK/
|   +-- src/
|   +-- .project
|   +-- .cproject
|   +-- Parking_System_project.mex
|
+-- CAN_Database/
|   +-- Reverse_Parking_Assist.dbc
|
+-- Screenshots/
|   +-- 01_Project_Structure.png
|   +-- 02_CANdb_DBC_Design.png
|   +-- 03_TSMaster_Configuration.png
|   +-- 04_TSMaster_CAN_Trace.png
|   +-- 05_S32DS_Firmware.png
|   +-- 06_Output_Testing.png
|
+-- Documentation/
|   +-- Test_Cases.md
|
+-- README.md
```

Adjust the folder names to match the files actually included in the repository. Include SDK files only when permitted by their licensing terms and needed to reproduce the project.

## 15. Project Status

**Automotive embedded ECU prototype with CAN database development and TSMaster-based simulation.**

The scope of completed validation should reflect the tests actually performed. This project is an educational prototype and is not a production-certified parking assistance system.

## Author

**Sivaarunmani G K**

- GitHub: https://github.com/sivaarunmanigk
- LinkedIn: https://www.linkedin.com/in/sivaarunmanigk/
