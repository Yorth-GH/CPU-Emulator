#include "includes.h"

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

			if (operand1.length() == 16)
			{
				set_memory(temp_address++, 0x00);
				uint64_t direct_address = std::stoull(operand1, nullptr, 16);
				set_memory_64(temp_address, direct_address);
				temp_address += 8;
			}
			else if (operand1.at(0) == 'r')
			{
				set_memory(temp_address++, 0x01);
				set_memory(temp_address++, register_map.at(operand1));
			}
			else if (operand1.at(0) == '[')
			{
				if (operand1.length() == 4)
				{
					set_memory(temp_address++, 0x04);
					std::string operand1_indirect = operand1.substr(1, operand1.length() - 2);
					set_memory(temp_address++, register_map.at(operand1_indirect));
				}
				else if (operand1.length() == 18)
				{
					set_memory(temp_address++, 0x03);
					std::string operand1_indirect = operand1.substr(1, operand1.length() - 2);
					set_memory_64(temp_address, std::stoull(operand1_indirect, nullptr, 16));
					temp_address += 8;
				}
			}
			else
			{
				set_memory(temp_address++, 0x02);
				set_memory_64(temp_address, std::stoull(operand1, nullptr, 16));
				temp_address += 8;
			}

			if (opcode_map.at(instruction) == 0x07 || opcode_map.at(instruction) == 0x10 || opcode_map.at(instruction) == 0x11)
				continue;

			if (operand2.length() == 16)
			{
				set_memory(temp_address++, 0x00);
				uint64_t direct_address = std::stoull(operand2, nullptr, 16);
				set_memory_64(temp_address, direct_address);
				temp_address += 8;
			}
			else if (operand2.at(0) == 'r')
			{
				set_memory(temp_address++, 0x01);
				set_memory(temp_address++, register_map.at(operand2));
			}
			else if (operand2.at(0) == '[')
			{
				if (operand2.length() == 4)
				{
					set_memory(temp_address++, 0x04);
					std::string operand2_indirect = operand2.substr(1, operand2.length() - 2);
					set_memory(temp_address++, register_map.at(operand2_indirect));
				}
				else if (operand2.length() == 18)
				{
					set_memory(temp_address++, 0x03);
					std::string operand2_indirect = operand2.substr(1, operand2.length() - 2);
					set_memory_64(temp_address, std::stoull(operand2_indirect, nullptr, 16));
					temp_address += 8;
				}
			}
			else
			{
				set_memory(temp_address++, 0x02);
				set_memory_64(temp_address, std::stoull(operand2, nullptr, 16));
				temp_address += 8;
			}
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
