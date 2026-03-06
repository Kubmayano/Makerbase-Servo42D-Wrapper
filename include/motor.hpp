#pragma once

#include <stdint.h>
#include "modbusbus.hpp"



/**
 * @brief A class containing methods for operation and control of the motors.
 */
class Motor
{
    public:
        /**
         * @brief Constructor for each motor.
         * @param bus A ModbusBus reference, each motor shares the same singular reference.
         * @param unit The unique motor ID, this begins at 0 but is reserved, range is 1->255.
         */
        Motor(ModbusBus& bus, uint8_t unit);

        /**
         * @brief Sets motor RPM.
         * @param direction 0 -> CCW, 1 -> CW.
         * @param acceleration 0 -> 255, 255 being almost instant.
         * @param speed 0 -> 3000.
         * @return Number of registers written from modbus, Returns -1 if failed.
         */
        int set_rpm(uint8_t direction, uint8_t acceleration, uint16_t speed);

        /**
         * @brief Sets motor RPM for a given period of time before it automatically stops.
         * @param direction 0 -> CCW, 1 -> CW.
         * @param acceleration 0 -> 255, 255 being almost instant.
         * @param speed 0 -> 3000.
         * @param runtime Runtime is given in steps of 10ms, maximum is 49.71 days.
         * @return Number of registers written from modbus, Returns -1 if failed.
         */
        int set_rpm_timed(uint8_t direction, uint8_t acceleration, uint16_t speed, uint32_t runtime);

        /**
         * @brief Stops the motor.
         * @param acceleration 8 bit unsigned integer, 0 is instant, 255 is the slowest.
         * @return Number of registers written from modbus, Returns -1 if failed.
         */
        int stop(uint8_t acceleration);

        /**
         * @brief Stops the motor instantly, should avoid using over 1000RPM.
         * @return Number of registers written from modbus, Returns -1 if failed.
         */
        int emergency_stop();

        /**
         * @brief Reads the realtime speed of the motor in RPM.
         * @return 16 bit signed integer, negative means motor is rotating in reverse.
         */
        int16_t read_realtime_speed();

        /**
         * @brief Reads the position angle of the motor.
         * @return Realtime angle error represented by 0->51200, Angle is calculated as 51200/142.22.
         */
        int32_t read_position_angle_error();

    private:
        ModbusBus& bus;
        uint8_t unit;

};