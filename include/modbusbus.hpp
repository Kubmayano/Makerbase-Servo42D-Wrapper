#pragma once

#include <string>
#include <modbus/modbus.h>

class ModbusBus
{
public:
    explicit ModbusBus(const std::string& port, int baud);
    ~ModbusBus();
    int read_holding_regs(uint8_t unit, int addr, int nb, uint16_t* dest);
    int write_single_reg(uint8_t unit, int addr, uint16_t value);
    int write_multiple_registers(uint8_t unit, int addr, int nb, const uint16_t* values);

    // modbus_t* ctx() const { return ctx_; } // optional accessor

private:
    modbus_t* ctx_ = nullptr;
};