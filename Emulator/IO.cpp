#include "includes.h"

bool CPU::load_operand(std::string operand, uint64_t& temp_address)
{

	if (operand.length() == 16) // address
	{
		set_memory(temp_address++, 0x00);
		uint64_t direct_address = std::stoull(operand, nullptr, 16);
		set_memory_64(temp_address, direct_address);
		temp_address += 8;
	}
	else if (operand.at(0) == 'r') // register
	{
		set_memory(temp_address++, 0x01);
		set_memory(temp_address++, register_map.at(operand));
	}
	else if (operand.at(0) == '[') // indirect
	{
		if (operand.length() == 4) // register
		{
			set_memory(temp_address++, 0x04);
			std::string operand1_indirect = operand.substr(1, operand.length() - 2);
			set_memory(temp_address++, register_map.at(operand1_indirect));
		}
		else if (operand.length() == 18) // address
		{
			set_memory(temp_address++, 0x03);
			std::string operand1_indirect = operand.substr(1, operand.length() - 2);
			set_memory_64(temp_address, std::stoull(operand1_indirect, nullptr, 16));
			temp_address += 8;
		}
	}
	else if (!(!operand.empty() && std::all_of(operand.begin(), operand.end(), ::isdigit)))
	{
		std::cout << "ERROR ??INVALID OPERAND?? ERROR" << std::endl << "Operand: " << operand << std::endl;
		return false;
	}
	else // immediate value
	{
		set_memory(temp_address++, 0x02);
		set_memory_64(temp_address, std::stoull(operand, nullptr, 16));
		temp_address += 8;
	}
	return true;
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

			if (opcode_map.at(instruction) == 0x12) // halt, no operands
				continue;

			if (!load_operand(operand1, temp_address))
				return false;

			if (opcode_map.at(instruction) == 0x07 || opcode_map.at(instruction) == 0x10 || opcode_map.at(instruction) == 0x11 ||
				opcode_map.at(instruction) == 0x0a || opcode_map.at(instruction) == 0x0b || opcode_map.at(instruction) == 0x0c ||
				opcode_map.at(instruction) == 0x0d || opcode_map.at(instruction) == 0x0e) // single operand instructions
				continue;

			if (!load_operand(operand2, temp_address))
				return false;
		}
		else
		{
			std::cout << "ERROR ??INVALID INSTRUCTION?? ERROR" << std::endl << "Instruction: " << instruction << std::endl;
			return false;
		}
	}
	return true;
}

void CPU::in(uint64_t reg)
{
	uint64_t input;
	std::cin >> input;
	set_register(reg, input);
}

void CPU::out(uint64_t reg)
{
	std::cout << "Register R" << reg-240 << " value is: 0x" << std::hex << get_register(reg) << std::endl;
}
