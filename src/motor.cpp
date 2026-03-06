#include "modbusbus.hpp"
#include <stdint.h>
#include <stdexcept>


class Motor
{
    public:
        Motor(ModbusBus& bus, uint8_t unit) : bus(bus), unit(unit)
        {

        }

        int set_rpm(uint8_t direction, uint8_t acceleration, uint16_t speed)
        {
            int addr = 0x00F6;
            int nr = 0x02;
            uint16_t data[nr];

            if (direction > 1) direction = 1;
            if (acceleration > 255) acceleration = 255;
            if (speed > 3000) speed = 3000;

            data[0] = (direction << 8) | acceleration;
            data[1] = speed;

            return bus.write_multiple_registers(unit, addr, nr, data);
        }

        /*
        Time works in intervals of 10ms, where the max val for runtime is: 0x19990000
        Expands to:
        429 457 408
        Multiplied with time interval
        429457408 * 10ms = 49.71 days
        */
        int set_rpm_timed(uint8_t direction, uint8_t acceleration, uint16_t speed, uint32_t runtime)
        {
            int addr = 0x00F6;
            int nr = 0x04;
            uint16_t data[nr];

            if (direction > 1) direction = 1;
            if (acceleration > 255) acceleration = 255;
            if (speed > 3000) speed = 3000;
            if (runtime > 0x19990000)runtime = 0x19990000;

            data[0] = (direction << 8) | acceleration;
            data[1] = speed;
            data[2] = (runtime >> 16);
            data[3] = (runtime & 0xFFFF);

            return bus.write_multiple_registers(unit, addr, nr, data);
        }

        int stop(uint8_t acceleration)
        {
            int addr = 0x00F6;
            int nr = 0x02;
            uint16_t data[nr];
            
            if (acceleration > 255) acceleration = 255;

            data[0] = acceleration; //can be written as (0x00 << 8) | acceleration;
            data[1] = 0x00;

            return bus.write_multiple_registers(unit, addr, nr, data);
        }

        int emergency_stop()
        {
            int addr = 0x00F6;
            int nr = 0x02;
            uint16_t data[nr];

            data[0] = 0x00;
            data[1] = 0x00;

            return bus.write_multiple_registers(unit, addr, nr, data);
        }

        int16_t read_realtime_speed()
        {
            int addr = 0x0032;
            int nr = 0x01;

            uint16_t res = 0;

            int res_count = bus.read_input_registers(unit, addr, nr, &res);
            if (res_count != nr)
            {
                throw std::runtime_error("modbus read returned unexpected register count");
            }

            return static_cast<int16_t>(res);
        }

        int32_t read_position_angle_error()
        {
            int addr = 0x0039;
            int nr = 0x02;

            uint16_t res[nr];

            int res_count = bus.read_input_registers(unit, addr, nr, res);
            if (res_count != nr)
            {
                throw std::runtime_error("modbus read returned unexpected register count");
            }

            return static_cast<int32_t>(res[0] << 16 | res[1]);
        }

    private:
        ModbusBus& bus;
        uint8_t unit;
};