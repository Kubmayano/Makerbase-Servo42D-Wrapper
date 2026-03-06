#include "motor.hpp"
#include "modbusbus.hpp"

#include <stdint.h>
#include <stdexcept>
#include <iostream>

Motor::Motor(ModbusBus& bus, uint8_t unit) : bus(bus), unit(unit)
{

}

Motor::~Motor()
{
    
}

int Motor::set_rpm(uint8_t direction, uint8_t acceleration, uint16_t speed)
{
    constexpr int addr = 0x00F6;
    constexpr int nr = 0x02;
    uint16_t data[nr];

    if (direction > 1) direction = 1;
    if (acceleration > 255) acceleration = 255;
    if (speed > 3000) speed = 3000;

    data[0] = (direction << 8) | acceleration;
    data[1] = speed;

    int res_count = bus.write_multiple_registers(unit, addr, nr, data);
    if (res_count != nr)
    {
        std::cerr << "Failed wrting to registers\n";
        return -1;
    }

    return 0;
}

int Motor::set_rpm_timed(uint8_t direction, uint8_t acceleration, uint16_t speed, uint32_t runtime)
{
    constexpr int addr = 0x00F6;
    constexpr int nr = 0x04;
    uint16_t data[nr];

    if (direction > 1) direction = 1;
    if (acceleration > 255) acceleration = 255;
    if (speed > 3000) speed = 3000;
    if (runtime > 0x19990000)runtime = 0x19990000;

    data[0] = (direction << 8) | acceleration;
    data[1] = speed;
    data[2] = (runtime >> 16);
    data[3] = (runtime & 0xFFFF);

    int res_count = bus.write_multiple_registers(unit, addr, nr, data);
    if (res_count != nr)
    {
        std::cerr << "Failed writing to registers\n";
        return -1;
    }

    return 0;
}

int Motor::stop(uint8_t acceleration)
{
    constexpr int addr = 0x00F6;
    constexpr int nr = 0x02;
    uint16_t data[nr];

    if (acceleration > 255)
        acceleration = 255;

    data[0] = acceleration; // can be written as (0x00 << 8) | acceleration;
    data[1] = 0x00;

    int res_count = bus.write_multiple_registers(unit, addr, nr, data);
    if (res_count != nr)
    {
        std::cerr << "Failed writing to registers\n";
        return -1;
    }

    return 0;
}

int Motor::emergency_stop()
{
    constexpr int addr = 0x00F7;
    uint16_t data = 0x01;

    if(bus.write_single_register(unit, addr, data) == -1)
    {
        std::cerr << "Failed writing to register\n";
        return -1;
    }

    return 0;
}

int16_t Motor::read_realtime_speed()
{
    constexpr int addr = 0x0032;
    constexpr int nr = 0x01;

    uint16_t res = 0;

    int res_count = bus.read_input_registers(unit, addr, nr, &res);
    if (res_count != nr)
    {
        throw std::runtime_error("modbus read returned unexpected register count");
    }

    return static_cast<int16_t>(res);
}

int32_t Motor::read_position_angle_error()
{
    constexpr int addr = 0x0039;
    constexpr int nr = 0x02;

    uint16_t res[nr];

    int res_count = bus.read_input_registers(unit, addr, nr, res);
    if (res_count != nr)
    {
        throw std::runtime_error("modbus read returned unexpected register count");
    }

    return static_cast<int32_t>(res[0] << 16 | res[1]);
}

bool Motor::read_enable_status()
{
    constexpr int addr = 0x003A;
    constexpr int nr = 0x01;
    uint16_t res = 0;

    int res_count = bus.read_input_registers(unit, addr, nr, &res);
    if (res_count != nr)
    {
        throw std::runtime_error("modbus read returned unexpected register count");
    }

    return static_cast<bool>(res & 0xFF);
}

uint32_t Motor::read_version_information()
{
    constexpr int addr = 0x0040;
    constexpr int nr = 0x02;
    uint16_t res[nr];

    int res_count = bus.read_input_registers(unit, addr, nr, res);
    if (res_count != nr)
    {
        throw std::runtime_error("modbus read returned unexpected register count");
    }

    return static_cast<uint32_t>(res[0] << 16 | res[1]);
}

int Motor::calibrate_motor()
{
    constexpr int addr = 0x0080;
    uint16_t data = 0x0001;

    if(bus.write_single_register(unit, addr, data) == -1)
    {
        std::cerr << "Failed writing to register\n";
        return -1;
    }

    return 0;
}

int Motor::set_work_mode(uint16_t mode)
{
    constexpr int addr = 0x0082;
    uint16_t data = mode;

    if(bus.write_single_register(unit, addr, data) == -1)
    {
        std::cerr << "Failed writing to register\n";
        return -1;
    }

    return 0;
}

int Motor::set_working_current(uint16_t current)
{
    constexpr int addr = 0x0083;
    uint16_t data = current;

    if(bus.write_single_register(unit, addr, data) == -1)
    {
        std::cerr << "Failed writing to register\n";
        return -1;
    }

    return 0;
}

int Motor::set_holding_current(uint8_t hold_option)
{
    constexpr int addr = 0x009B;
    uint16_t data = hold_option;

    if (bus.write_single_register(unit, addr, data) == -1)
    {
        std::cerr << "Failed writing to register\n";
        return -1;
    }

    return 0;
}

int Motor::set_subdivisions(uint8_t step_size)
{
    constexpr int addr = 0x0084;
    uint16_t data = step_size;

    if (bus.write_single_register(unit, addr, data) == -1)
    {
        std::cerr << "Failed writing to register\n";
        return -1;
    }

    return 0;
}

int Motor::automatic_screen_off(uint8_t enable)
{
    constexpr int addr = 0x0087;
    if (enable > 1) enable == 1;
    uint16_t data = enable;

    if (bus.write_single_register(unit, addr, data) == -1)
    {
        std::cerr << "Failed writing to register\n";
        return -1;
    }

    return 0;
}

int Motor::set_subdivision_interpolation(uint8_t enable)
{
    constexpr int addr = 0x0089;
    if (enable > 1) enable = 1;
    uint16_t data = enable;

    if (bus.write_single_register(unit, addr, data) == -1)
    {
        std::cerr << "Failed writing to register\n"; 
        return -1;
    }

    return 0;
}

int Motor::set_baud_rate(uint8_t baud)
{
    constexpr int addr = 0x008A;
    if (baud > 7) baud = 7;
    uint16_t data = baud;

    if (bus.write_single_register(unit, addr, data) == -1)
    {
        std::cerr << "Failed writing to register\n";
        return -1;
    }

    return 0;
}

int Motor::set_slave_addr(uint8_t address)
{
    constexpr int addr = 0x008B;
    if (address == 0)
    {
        std::cerr << "Unable to set 0 as address, taken by broadcast address\n";
        return -1;
    }
    uint16_t data = address;

    if (bus.write_single_register(unit, addr, data) == -1)
    {
        std::cerr << "Failed to write to register\n";
        return -1;
    }

    return 0;
}

int Motor::set_PID_parameters_vFOC(uint16_t Kp, uint16_t Ki, uint16_t Kd, uint16_t Kv)
{
    constexpr int addr = 0x0096;
    constexpr int nr = 0x04;
    if (Kp > 1024) Kp = 1024;
    if (Ki > 1024) Ki = 1024;
    if (Kd > 1024) Kd = 1024;
    if (Kv > 1024) Kv = 1024;

    uint16_t data[nr];
    data[0] = Kp;
    data[1] = Ki;
    data[2] = Kd;
    data[3] = Kv;

    int res_count = bus.write_multiple_registers(unit, addr, nr, data);
    if (res_count != nr)
    {
        std::cerr << "modbus write returned unexpected register count\n";
        return -1;
    }

    return 0;
}