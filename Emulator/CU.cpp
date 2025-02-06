#include "includes.h"

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
		operand1 = fetch();
		break;

	case 0x09:
		operand1 = 0;
		operand2 = 0;

		op1_marker = fetch();
		if (op1_marker == 0x01 || op1_marker == 0x04)
			operand1 = fetch();
		else
			operand1 = fetch_64();


		op2_marker = fetch();
		if (op2_marker == 0x01 || op2_marker == 0x04)
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
		set_register(operand1, add(operand1, operand2));
		break;

	case 0x02:
		set_register(operand1, sub(operand1, operand2));
		break;

	case 0x03:
		set_register(operand1, mul(operand1, operand2));
		break;

	case 0x04:
		set_register(operand1, div(operand1, operand2));
		break;

	case 0x05:
		set_register(operand1, BINand(operand1, operand2));
		break;

	case 0x06:
		set_register(operand1, BINor(operand1, operand2));
		break;

	case 0x07:
		if (op1_marker != 0x01)
			std::cout << "ERROR ??OPERAND NOT A REGISTER?? ERROR" << std::endl;
		set_register(operand1, BINnot(operand1));
		break;

	case 0x08:
		set_register(operand1, BINxor(operand1, operand2));
		break;

	case 0x09:
		switch (op1_marker)
		{
		case 0x00: case 0x02:
			std::cout << "ERROR ??OPERAND IS VALUE?? ERROR" << std::endl;
			break;
		case 0x01:
			switch (op2_marker)
			{
			case 0x00: case 0x02:
				set_register(operand1, operand2);
				break;
			case 0x01:
				set_register(operand1, get_register(operand2));
				break;
			case 0x03:
				set_register(operand1, get_memory_64(operand2));
				break;
			case 0x04:
				set_register(operand1, get_memory_64(get_register(operand2)));
				break;
			}
			break;
		case 0x03:
			switch (op2_marker)
			{
			case 0x00: case 0x02:
				set_memory_64(operand1, operand2);
				break;
			case 0x01:
				set_memory_64(operand1, get_register(operand2));
				break;
			case 0x03:
				set_memory_64(operand1, get_memory_64(operand2));
				break;
			case 0x04:
				set_memory_64(operand1, get_memory_64(get_register(operand2)));
				break;
			}
			break;
		case 0x04:
			switch (op2_marker)
			{
			case 0x00: case 0x02:
				set_register(operand1, operand2);
				break;
			case 0x01:
				set_register(operand1, get_register(operand2));
				break;
			case 0x03:
				set_register(operand1, get_memory_64(operand2));
				break;
			case 0x04:
				break;
			}
			break;
		}
		break;

	case 0x10:
		if (op1_marker != 0x01)
			std::cout << "ERROR ??OPERAND NOT A REGISTER?? ERROR" << std::endl;
		in(operand1);
		break;

	case 0x11:
		if (op1_marker != 0x01)
			std::cout << "ERROR ??OPERAND NOT A REGISTER?? ERROR" << std::endl;
		out(operand1);
		break;

	case 0x12:
		halted = true;
		break;
	}
}
