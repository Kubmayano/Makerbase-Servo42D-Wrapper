#pragma once

#include <stdint.h>

class Motor
{
    public:
        Motor(ModbusBus& bus, uint8_t unit) : bus(bus), unit(unit);
        int set_rpm(uint8_t direction, uint8_t acceleration, uint16_t speed);
        int set_rpm_timed(uint8_t direction, uint8_t acceleration, uint16_t speed, uint32_t runtime);
        int stop(uint8_t acceleration);
        int emergency_stop();

    private:
        ModbusBus& bus;
        uint8_t unit;

};