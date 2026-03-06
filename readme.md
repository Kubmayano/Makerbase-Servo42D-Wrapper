# MAKERBASE SERVO42D C++ DRIVER

A custom made C++ driver for controlling the **Makerbase SERVO42D motor**.
This driver contains functions for reading motor telemetry and controlling the motor.

## Features
- Reading real-time motor speed.
- Reading real-time encoder position.
- Reading real-time tracking error.
- Emergency stop command
- Thread-safe Modbus access with std::mutex.
- Cleanly written driver using the 'libmodbus' C library.

---

## Requirements

- Working C++ compiler, recommend G++. 
- 'libmodbus' C library.

Steps for installing working C++ compiler, and CMake on Linux:
```bash
sudo apt update
sudo apt install build-essential
sudo apt install cmake
```

Steps for installing libmodbus on Linux:

```bash
sudo apt update
sudo apt install libmodbus-dev
```

Program can be ran with 