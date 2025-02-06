#include "includes.h"

uint64_t CPU::get_program_counter()
{
	return program_counter;
}
uint64_t CPU::get_register(uint64_t reg)
{
	int register_num = reg - 240;
	if (register_num >= 0 && register_num < 4)
		return registers[register_num];
	else
		return UINT64_MAX;;
}
bool CPU::set_register(uint64_t reg, uint64_t data)
{
	int register_num = reg - 240;
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
uint64_t CPU::add(uint64_t a, uint64_t b)
{
	return CPU::registers[a-240] + CPU::registers[b-240];
}
uint64_t CPU::sub(uint64_t a, uint64_t b)
{
	return CPU::registers[a - 240] - CPU::registers[b - 240];
}
uint64_t CPU::mul(uint64_t a, uint64_t b)
{
	return CPU::registers[a - 240] * CPU::registers[b - 240];
}
uint64_t CPU::div(uint64_t a, uint64_t b)
{
	return CPU::registers[a - 240] * CPU::registers[b - 240];
}
uint64_t CPU::BINand(uint64_t a, uint64_t b)
{
	return CPU::registers[a - 240] & CPU::registers[b - 240];
}
uint64_t CPU::BINor(uint64_t a, uint64_t b)
{
	return CPU::registers[a - 240] | CPU::registers[b - 240];
}
uint64_t CPU::BINnot(uint64_t a)
{
	return ~CPU::registers[a - 240];
}
uint64_t CPU::BINxor(uint64_t a, uint64_t b)
{
	return CPU::registers[a - 240] * CPU::registers[b - 240];
}

bool CPU::load_program(std::string file)
{
	std::ifstream program_file(file);
	if (!program_file.is_open())
		return false;

	std::string line;
	uint64_t temp_address = 0;

	while (std::getline(program_file, line))
	{	
		std::istringstream iss(line);
		std::string instruction, operand1, operand2;
		iss >> instruction >> operand1 >> operand2;

		if (opcode_map.count(instruction))
		{
			set_memory(temp_address++, opcode_map.at(instruction));

			if (opcode_map.at(instruction) == 0x12)
				continue;

			if (operand1.length() == 16) // address
			{
#ifdef _DEBUG 
				std::cout << "o1 - address" << std::endl;
#endif
				set_memory(temp_address++, 0x00);
				uint64_t direct_address = std::stoull(operand1, nullptr, 16);
				set_memory_64(temp_address, direct_address);
				temp_address += 8;
			}
			else if (operand1.at(0) == 'r') // register
			{
#ifdef _DEBUG 
				std::cout << "o1 - register" << std::endl;
#endif
				set_memory(temp_address++, 0x01);
				set_memory(temp_address++, register_map.at(operand1));
			}
			else if (operand1.at(0) == '[') // indirect
			{
#ifdef _DEBUG 
				std::cout << "o1 - indirect" << std::endl;
#endif
				set_memory(temp_address++, 0x03);
				std::string operand1_indirect = operand1.substr(1, operand1.length() - 2);
				set_memory_64(temp_address, std::stoull(operand1_indirect, nullptr, 16));
				temp_address += 8;
			}
			else // immediate
			{
#ifdef _DEBUG 
				std::cout << "o1 - immediate" << std::endl;
#endif
				set_memory(temp_address++, 0x02);
				set_memory_64(temp_address, std::stoull(operand1, nullptr, 16));
				temp_address += 8;
			}

			if (opcode_map.at(instruction) == 0x07 || opcode_map.at(instruction) == 0x10 || opcode_map.at(instruction) == 0x11)
				continue;

			if (operand2.length() == 16) //address
			{
#ifdef _DEBUG 
				std::cout << "o2 - address" << std::endl;
#endif
				set_memory(temp_address++, 0x00);
				uint64_t direct_address = std::stoull(operand2, nullptr, 16);
				set_memory_64(temp_address, direct_address);
				temp_address += 8;
			}
			else if (operand2.at(0) == 'r') // register
			{
#ifdef _DEBUG 
				std::cout << "o2 - register" << std::endl;
#endif
				set_memory(temp_address++, 0x01);
				set_memory(temp_address++, register_map.at(operand2));
			}
			else if (operand2.at(0) == '[') // indirect
			{
#ifdef _DEBUG 
				std::cout << "o2 - indirect" << std::endl;
#endif
				set_memory(temp_address++, 0x03);
				std::string operand2_indirect = operand2.substr(1, operand2.length() - 2);
				set_memory_64(temp_address, std::stoull(operand2_indirect, nullptr, 16));
				temp_address += 8;
			} 
			else // immediate
			{
#ifdef _DEBUG 
				std::cout << "o2 - immediate" << std::endl;
#endif
				set_memory(temp_address++, 0x02);
				set_memory_64(temp_address, std::stoull(operand2, nullptr, 16));
				temp_address += 8;
			}

		}
	}
	return true;
}

bool CPU::get_halted()
{
	return CPU::halted;
}
uint64_t CPU::fetch_64()
{
	uint64_t value = 0;
	for (int i = 0; i < 8; i++)
		value |= (uint64_t)((fetch()) << (i * 8));
	return value;
}
uint8_t CPU::fetch()
{
	return memory[program_counter++];
}

void CPU::decode()
{
	CPU::opcode = fetch();

	switch (CPU::opcode) 
	{
	case 0x01: case 0x02: case 0x03: case 0x04:
	case 0x05: case 0x06: case 0x08: case 0x0f:
		op1_marker = fetch();
		if (op1_marker == 0x02)
			operand1 = fetch_64();
		else
			operand1 = fetch();

		op2_marker = fetch();
		if (op2_marker == 0x02)
			operand2 = fetch_64();
		else
			operand2 = fetch();
		break;

	case 0x07: case 0x10: case 0x11:
		op1_marker = fetch();
		if (op1_marker == 0x01)
			operand1 = fetch();
		else
			operand1 = fetch_64();
		break;

	case 0x09:
		operand1 = 0;
		operand2 = 0;

		op1_marker = fetch();
		if (op1_marker == 0x01)
			operand1 = fetch();
		else
			operand1 = fetch_64();
		

		op2_marker = fetch();
		if (op2_marker == 0x01)
			operand2 = fetch();
		else
			operand2 = fetch_64();

		break;

	case 0x12:
		break;
	}
}

void CPU::execute()
{
	switch (opcode)
	{
	case 0x01:
#ifdef _DEBUG 
		std::cout << "op - add" << std::endl;
#endif
		set_register(operand1, add(operand1, operand2));
		break;

	case 0x02:
#ifdef _DEBUG 
		std::cout << "op - sub" << std::endl;
#endif
		set_register(operand1, sub(operand1, operand2));
		break;

	case 0x03:
#ifdef _DEBUG 
		std::cout << "op - mul" << std::endl;
#endif
		set_register(operand1, mul(operand1, operand2));
		break;

	case 0x04:
#ifdef _DEBUG 
		std::cout << "op - div" << std::endl;
#endif
		set_register(operand1, div(operand1, operand2));
		break;

	case 0x05:
#ifdef _DEBUG 
		std::cout << "op - BINand" << std::endl;
#endif
		set_register(operand1, BINand(operand1, operand2));
		break;

	case 0x06:
#ifdef _DEBUG 
		std::cout << "op - BINor" << std::endl;
#endif
		set_register(operand1, BINor(operand1, operand2));
		break;

	case 0x07:
#ifdef _DEBUG 
		std::cout << "op - BINnot" << std::endl;
#endif
		set_register(operand1, BINnot(operand1));
		break;

	case 0x08:
#ifdef _DEBUG 
		std::cout << "op - BINxor" << std::endl;
#endif
		set_register(operand1, BINxor(operand1, operand2));
		break;

	case 0x09:
#ifdef _DEBUG 
		std::cout << "op - mov" << std::endl;
#endif
 		if (op1_marker == 0x01 && op2_marker == 0x01)
			set_register(operand1, get_register(operand2));
		else if (op1_marker == 0x01 && op2_marker != 0x01)
		{
			if (op2_marker == 0x00)
				set_register(operand1, get_memory_64(operand2));
			else
				set_register(operand1, operand2);
		}
		else if (op1_marker != 0x01 && op2_marker == 0x01)
		{
			// TODO: set all 8 bytes
			set_memory_64(operand1, get_register(operand2));
		}
		else if (op1_marker != 0x01 && op2_marker != 0x01)
		{
			if (op2_marker == 0x00)
				set_memory(operand1, get_memory_64(operand2));
			else
				set_memory(operand1, operand2);
		}
		break;

	case 0x10:
#ifdef _DEBUG 
		std::cout << "op - in" << std::endl;
#endif
		uint64_t input;
		std::cin >> input;
		set_register(operand1, input);
		break;

	case 0x11:
#ifdef _DEBUG 
		std::cout << "op - out" << std::endl;
#endif
		std::cout << get_register(operand1) << std::endl;
		break;

	case 0x12:
#ifdef _DEBUG 
		std::cout << "op - halt" << std::endl;
#endif
		halted = true;
		break;
	}
}