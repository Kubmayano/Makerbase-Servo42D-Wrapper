#include "modbusbus.hpp"
#include <stdexcept>
#include <mutex>

class ModbusBus
{
    public:
        explicit ModbusBus(const std::string& port, int baud)
        {
            ctx_ = modbus_new_rtu(port.c_str(), baud, 'N', 8, 1);
            if (!ctx_) throw std::runtime_error("modbus_new_rtu failed");
            if (modbus_connect(ctx_) != 0) throw std::runtime_error("modbus connection failed"); 
        }

        ~ModbusBus()
        {
            if (ctx_) modbus_close(ctx_);
            modbus_free(ctx_);
        }

        int read_holding_regs(uint8_t unit, int addr, int nb, uint16_t* dest)
        {
            std::scoped_lock lk(mtx_);
            modbus_set_slave(ctx_, unit);
            return modbus_read_registers(ctx_, addr, nb, dest);
        }

        int write_single_reg(uint8_t unit, int addr, uint16_t value)
        {
            std::scoped_lock lk(mtx_);
            modbus_set_slave(ctx_, unit);
            return modbus_write_register(ctx_, addr, value);
        }

        int write_multiple_registers(uint8_t unit, int addr, int nb, const uint16_t* values)
        {
            std::scoped_lock lk(mtx_);
            modbus_set_slave(ctx_, unit);
            return modbus_write_registers(ctx_, addr, nb, values);
        }

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