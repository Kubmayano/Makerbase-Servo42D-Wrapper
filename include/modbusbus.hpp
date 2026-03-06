#pragma once

#include <string>
#include <modbus/modbus.h>
#include <mutex>

class ModbusBus
{
    public:
        explicit ModbusBus(const std::string& port, int baud);
        ~ModbusBus();

        /**
         * @brief Reads the value of holding registers.
         * @param unit The motor ID which to execute the command on.
         * @param addr The function address according to servo42d documentation.
         * @param nb The number of registers being read.
         * @param dest The destination array to send the data back to.
         * @return Not sure about this yet.
         */
        int read_holding_registers(uint8_t unit, int addr, int nb, uint16_t* dest);

        /**
         * @brief Writes data to a single register.
         * @param unit The motor ID which to execute the command on. 
         * @param addr The function address according to servo42d documentation.
         * @param value The value to write to the register.
         * @return Not sure about this yet.
         */
        int write_single_register(uint8_t unit, int addr, uint16_t value);

        /**
         * @brief Writes data to multiple registers.
         * @param unit The motor ID which to execute the command on.
         * @param addr The function address according to the servo42d documentation.
         * @param nb The number of registers being written to.
         * @param values The values being written to the registers.
         * @return Not sure about this yet.
         */
        int write_multiple_registers(uint8_t unit, int addr, int nb, const uint16_t* values);

        /**
         * @brief Reads the value of input registers.
         * @param unit The motor ID which to execute the command on.
         * @param addr The function address acording to the servo42d documentation.
         * @param nb The number of registers being read.
         * @param dest The destination array to write the data to.
         * @return Not sure about this yet.
         */
        int read_input_registers(uint8_t unit,int addr, int nb, uint16_t* dest);

    private:
        modbus_t* ctx_ = nullptr;
        std::mutex mtx_;

        static constexpr int READ_COILS = 0x01; 
        static constexpr int READ_DISCRETE_INPUTS = 0x02;
        static constexpr int READ_HOLDING_REGISTERS = 0x03;
        static constexpr int READ_INPUT_REGISTERS = 0x04;
        static constexpr int WRITE_SINGLE_COIL = 0x05;
        static constexpr int WRITE_SINGLE_REGISTER = 0x06;
        static constexpr int DIAGNOSTICS = 0x08; //Serial Line Only
        static constexpr int GET_COMM_EVENT_COUNTER = 0x0B; //Serial Line Only
        static constexpr int WRITE_MULTIPLE_COILS = 0x0F;
        static constexpr int WRITE_MULTIPLE_REGISTERS = 0x10;
        static constexpr int REPORT_SERVER_ID = 0x11; //Serial Line Only
        static constexpr int MASK_WRITE_REGISTER = 0x16;
        static constexpr int READ_WRITE_MULTIPLE_REGISTERS = 0x17;
        static constexpr int READ_DEVICE_IDENTIFICATION_DATA = 0x2B; //Or 0x0E
};