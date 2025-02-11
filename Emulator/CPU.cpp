#include "includes.h"

uint64_t CPU::get_pc()
{
	return program_counter;
}

uint64_t CPU::get_register(uint64_t reg)
{
	uint64_t register_num = reg - 240;
	if (register_num >= 0 && register_num < 5)
		return registers[register_num];
	else
		return UINT64_MAX;;
}

bool CPU::set_register(uint64_t reg, uint64_t data)
{
	uint64_t register_num = reg - 240;
	if (register_num >= 0 && register_num < 5)
	{
		registers[register_num] = data;
		return true;
	}
	else
		return false;
}

bool CPU::set_memory(uint64_t address, uint8_t data)
{
	uint64_t page_number = address >> 12;
	uint64_t offset = address & 0xFFF;

	if (memory.find(page_number) == memory.end())
		memory[page_number] = new uint8_t[page_size];

	memory[page_number][offset] = data;
	return true;

	//memory[address] = data
	//return true;
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
	uint64_t page_number = address >> 12;
	uint64_t offset = address & 0xFFF;

	if (memory.find(page_number) == memory.end())
		return 0;

	return memory[page_number][offset];

	//return memory[address]
}

uint64_t CPU::get_memory_64(uint64_t address)
{
	uint64_t temp = address;
	uint64_t value = 0;
	for (int i = 0; i < 8; i++)
		value |= (uint64_t)(get_memory(temp++) << (i * 8));
	return value;
}

void CPU::reset()
{
	memory.clear();

	for (uint64_t reg : registers)
		reg = 0;

	program_counter = 0;

	opcode = 0;
	operand1 = 0, operand2 = 0;
	op1_marker = 0, op2_marker = 0;
	halted = false;
}