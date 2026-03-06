#include "include/motor.hpp"
#include "include/modbusbus.hpp"

#include <iostream>

int main()
{
    int baud_rate = 38400;
    std::string com_port = "COM3";

    ModbusBus bus(com_port, baud_rate);
}