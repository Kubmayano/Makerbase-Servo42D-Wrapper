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
         * @param slave_id The unique motor ID, this begins at 0 but is reserved, range is 1->255.
         */
        Motor(ModbusBus& bus, uint8_t slave_id);

        /**
         * @brief Empty destructor for now
         */
        ~Motor();

        /**
         * @brief Sets motor RPM.
         * @param direction 0 -> CCW, 1 -> CW.
         * @param acceleration 0 -> 255, 255 being almost instant.
         * @param speed 0 -> 3000.
         * @return 0 on success, -1 on failure.
         */
        int set_rpm(uint8_t direction, uint8_t acceleration, uint16_t speed);

        /**
         * @brief Sets motor RPM for a given period of time before it automatically stops.
         * @param direction 0 -> CCW, 1 -> CW.
         * @param acceleration 0 -> 255, 255 being almost instant.
         * @param speed 0 -> 3000.
         * @param runtime Runtime is given in steps of 10ms, maximum is 49.71 days.
         * @return 0 on success, -1 on failure.
         */
        int set_rpm_timed(uint8_t direction, uint8_t acceleration, uint16_t speed, uint32_t runtime);

        /**
         * @brief Stops the motor.
         * @param acceleration 8 bit unsigned integer, 0 is instant, 255 is the slowest.
         * @return 0 on success, -1 on failure.
         */
        int stop(uint8_t acceleration);

        /**
         * @brief Stops the motor instantly, should avoid using over 1000RPM.
         * @return 0 on success, -1 on failure.
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

        /**
         * @brief Checks if motor is enabled or not.
         * @return False is motor is disabled, True if motor is enabled.
         */
        bool read_enable_status();

        /**
         * @brief Outputs the version information of the motor, unfortunately no way to decode it.
         * @return version information.
         */
        uint32_t read_version_information();

        /**
         * @brief Calibrates the motor, same as the 'cal' option on the display.
         * @return 0 on success, -1 on failure.
         */
        int calibrate_motor();

        /**
         * @brief Sets the working mode of the motor.
         * @param mode 00 => CR_OPEN, 01 => CR_CLOSE, 02 => CR_vFOC, 03 => SR_OPEN, 04 => SR_CLOSED, 05 => SR_vFOC.
         * @return 0 on success, -1 on failure.
         */
        int set_work_mode(uint16_t mode);

        /**
         * @brief Sets the working current of the motor, max 3000mA. If the highest bit of current is 1, the value will be set but not saved.
         * @param current The current in mA's.
         * @return 0 on success, -1 on failure.
         */
        int set_working_current(uint16_t current);

        /**
         * @brief Sets the holding current of the motor, max is 90% of working current.
         * @param hold_option 0 -> 10%, 1 -> 20%, 2 -> 30%, 3 -> 40%, 4 -> 50%, 5 -> 60%, 6 -> 70%, 7 -> 80%, 8 -> 90%
         * @return 0 on success, -1 on failure.
         */
        int set_holding_current(uint8_t hold_option);

        /**
         * @brief Sets the microstepping size.
         * @param step_size microstep size.
         * @return 0 on sucess, -1 on failure.
         */
        int set_subdivisions(uint8_t step_size);

        /**
         * @brief Enables or disables automatic screen off.
         * @param enable 0 for disable automatic screen off, 1 for enable.
         * @return 0 on success, -1 on failure.
         */
        int automatic_screen_off(uint8_t enable);

        /**
         * @brief Enables or disables subdivision interpolation, enabled makes for smoother turns.
         * @param enable 0 for disable interpolation, 1 for enable.
         * @return 0 on success, -1 on failure.
         */
        int set_subdivision_interpolation(uint8_t enable);

        /**
         * @brief Sets the serial port baud rate.
         * @param baud 1 -> 9600, 2 -> 19200, 3 -> 25000, 4 -> 38400, 5 -> 57600, 6 -> 115200, 7 -> 256000.
         * @return 0 on success, -1 on failure.
         */
        int set_baud_rate(uint8_t baud);

        /**
         * @brief Sets the current motor's slave address.
         * @param address 0 is used as the broadcast address, so 1 -> 255 are valid.
         * @return 0 on success, -1 on failure.
         */
        int set_slave_addr(uint8_t address);

        /**
         * @brief Tunes the PID parameters when motor is in vFOC mode, NOT ADVISED TO DO SO UNLESS STRICTLY NECESSARY.
         * @param Kp Range 0 -> 1024, default 0xDC.
         * @param Ki Range 0 -> 1024, default 0x64.
         * @param Kd Range 0 -> 1024, default 0x10E.
         * @param Kv Range 0 -> 1024, default 0x140.
         * @return 0 on success, -1 on failure.
         */
        int set_PID_parameters_vFOC(uint16_t Kp, uint16_t Ki, uint16_t Kd, uint16_t Kv);

    private:
        ModbusBus& bus;
        uint8_t slave_id;

};