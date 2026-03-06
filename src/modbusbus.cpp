#include "modbusbus.hpp"

#include <stdexcept>
#include <mutex>

ModbusBus::ModbusBus(const std::string& port, int baud)
{
    ctx_ = modbus_new_rtu(port.c_str(), baud, 'N', 8, 1);
    if (!ctx_) {
        throw std::runtime_error("modbus_new_rtu failed");
    }

    if (modbus_connect(ctx_) != 0) {
        std::string msg = "modbus connection failed: \n";
        msg += modbus_strerror(errno);

        modbus_free(ctx_);
        ctx_ = nullptr;

        throw std::runtime_error(msg);
    }
}

ModbusBus::~ModbusBus()
{
    if (ctx_) {
        modbus_close(ctx_);
        modbus_free(ctx_);
        ctx_ = nullptr;
    }
}

int ModbusBus::read_holding_registers(uint8_t unit, int addr, int nb, uint16_t* dest)
{
    std::scoped_lock lk(mtx_);
    modbus_set_slave(ctx_, unit);
    return modbus_read_registers(ctx_, addr, nb, dest);
}

int ModbusBus::read_input_registers(uint8_t unit, int addr, int nb, uint16_t* dest)
{
    std::scoped_lock lk(mtx_);
    modbus_set_slave(ctx_, unit);
    return modbus_read_input_registers(ctx_, addr, nb, dest);
}

int ModbusBus::write_single_register(uint8_t unit, int addr, uint16_t value)
{
    std::scoped_lock lk(mtx_);
    modbus_set_slave(ctx_, unit);
    return modbus_write_register(ctx_, addr, value);
}

int ModbusBus::write_multiple_registers(uint8_t unit, int addr, int nb, const uint16_t* values)
{
    std::scoped_lock lk(mtx_);
    modbus_set_slave(ctx_, unit);
    return modbus_write_registers(ctx_, addr, nb, values);
}