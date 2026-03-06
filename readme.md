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
- pkg-config

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

Steps for installing pkg-config on Linux:
```bash
sudo apt update
sudo apt install pkg-config
```

## Running the program
Clone project and run:
```bash
mkdir build
cd build
cmake ..
make
./servo42_example
```

## Known issues:
If program fails to run with errorcode
```bash
terminate called after throwing an instance of 'std::runtime_error'
  what():  modbus connection failed: 
Permission denied
Aborted (core dumped)
```
A fix can be done by ensuring the user is added to the dialout group, a temporary fix is:
```bash
newgrp dialout
make
```
This should fix the issue, look into how to add yourself to dialout group permanently online for a permanent fix.