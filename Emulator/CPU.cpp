#include "includes.h"

uint64_t CPU::get_register(uint64_t reg)
{
	uint64_t register_num = reg - 240;
	if (register_num >= 0 && register_num < 4)
		return registers[register_num];
	else
		return UINT64_MAX;;
}

bool CPU::set_register(uint64_t reg, uint64_t data)
{
	uint64_t register_num = reg - 240;
	if (register_num >= 0 && register_num < 4)
	{
		registers[register_num] = data;
		return true;
	}
	else
		return false;
}

bool CPU::set_memory(uint64_t address, uint8_t data)
{
	memory[address] = data;
	return true;
}

bool CPU::set_memory_64(uint64_t address, uint64_t data)
{
	uint64_t temp = address;
	for (int i = 0; i < 8; i++)
		set_memory(temp++, (uint8_t)(data >> (i * 8)));
	return true;
}

uint8_t CPU::get_memory(uint64_t address)
{
	return memory[address];
}

uint64_t CPU::get_memory_64(uint64_t address)
{
	uint64_t temp = address;
	uint64_t value = 0;
	for (int i = 0; i < 8; i++)
		value |= (uint64_t)(get_memory(temp++) << (i * 8));
	return value;
}
