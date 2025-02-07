#include "includes.h"

bool CPU::get_halted() const
{
	return CPU::halted;
}

uint64_t CPU::fetch_64()
{
	uint64_t value = 0;
	for (int i = 0; i < 8; i++)
		value |= ((uint64_t)(fetch()) << (i * 8));
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
	case 0x01: case 0x02: case 0x03: case 0x04:	// add, sub, mul, div
	case 0x05: case 0x06: case 0x08:			// and,  or, xor
		operand1 = 0;
		operand2 = 0;

		op1_marker = fetch();
		if (op1_marker != 0x01) // operand 1 not a register
		{
			std::cout << "ERROR ??OPERAND NOT A REGISTER?? ERROR" << std::endl;
			halted = true;
			break;
		}
		operand1 = fetch();

		op2_marker = fetch();
		if (op2_marker != 0x01) // operand 2 not a register
		{
			std::cout << "ERROR ??OPERAND NOT A REGISTER?? ERROR" << std::endl;
			halted = true;
			break;
		}
		operand2 = fetch();
		break;

	case 0x0f: // cmp
		operand1 = 0;
		operand2 = 0;

		op1_marker = fetch();
		if (op1_marker != 0x01 && op1_marker != 0x02) // operand 1 not a register or immediate value
		{
			std::cout << "ERROR ??OPERAND NOT A REGISTER OR IMMEDIATE VALUE?? ERROR" << std::endl;
			halted = true;
			break;
		}
		else if (op1_marker == 0x01)
			operand1 = fetch();
		else
			operand1 = fetch_64();

		op2_marker = fetch();
		if (op2_marker != 0x01 && op2_marker != 0x02) // operand 2 not a register or immediate value
		{
			std::cout << "ERROR ??OPERAND NOT A REGISTER OR IMMEDIATE VALUE?? ERROR" << std::endl;
			halted = true;
			break;
		}
		else if (op2_marker == 0x01)
			operand2 = fetch();
		else
			operand2 = fetch_64();
		break;

	case 0x07: case 0x10: case 0x11: // not, in, out
		operand1 = 0;
		operand2 = 0;

		op1_marker = fetch();
		if (op1_marker != 0x01) // operand 1 not a register
		{
			std::cout << "ERROR ??OPERAND NOT A REGISTER?? ERROR" << std::endl;
			halted = true;
			break;
		}
		operand1 = fetch();
		break;

	case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e: // jmp, je, jne, lge, jl
		operand1 = 0;
		operand2 = 0;

		op1_marker = fetch();
		if (op1_marker == 0x02 || op1_marker == 0x01) // operand 1 not an address or does not contain an address
		{
			std::cout << "ERROR ??OPERAND NOT AN ADDRESS?? ERROR" << std::endl;
			halted = true;
			break;
		}
		else if (op1_marker == 0x04) // operand 1 is a register containing an address
			operand1 = fetch();
		else if (op1_marker == 0x03 || op1_marker == 0x00) // operand 2 is an address containing an address or just an address
			operand1 = fetch_64();
		break;

	case 0x09: // mov
		operand1 = 0;
		operand2 = 0;

		op1_marker = fetch();
		if (op1_marker == 0x02)
		{
			std::cout << "ERROR ??OPERAND IS AN IMMEDIATE VALUE?? ERROR" << std::endl;
			halted = true;
			break;
		}
		else if (op1_marker == 0x01 || op1_marker == 0x04)
			operand1 = fetch();
		else
			operand1 = fetch_64();


		op2_marker = fetch();
		if ((op1_marker == 0x00 && op2_marker == 0x00) || (op1_marker == 0x03 && op2_marker == 0x00) || (op1_marker == 0x04 && op2_marker == 0x00))
		{
			std::cout << "ERROR ??MOVING ADDRESS INTO ADDRESS?? ERROR" << std::endl;
			fetch_64(); // unused, calling it so the program doesnt crash
			break;
		}
		else if (op2_marker == 0x01 || op2_marker == 0x04)
			operand2 = fetch();
		else
			operand2 = fetch_64();
		break;

	case 0x12: // halt
		break;
	}
}

void CPU::execute()
{
	switch (opcode)
	{
	case 0x01: // add
		set_register(operand1, add(operand1, operand2));
		break;

	case 0x02: // sub
		set_register(operand1, sub(operand1, operand2));
		break;

	case 0x03: // mul
		set_register(operand1, mul(operand1, operand2));
		break;

	case 0x04: // div
		set_register(operand1, div(operand1, operand2));
		break;

	case 0x05: // binary and
		set_register(operand1, BINand(operand1, operand2));
		break;

	case 0x06: // binary or
		set_register(operand1, BINor(operand1, operand2));
		break;

	case 0x07: // binary and
		set_register(operand1, BINnot(operand1));
		break;

	case 0x08: // binary xor
		set_register(operand1, BINxor(operand1, operand2));
		break;

	case 0x09: // mov
		switch (op1_marker)
		{
		case 0x02: // mov imm ( ry,addr,[ry],[addr], imm )
			break;
		case 0x00: // mov addr 
			switch (op2_marker)
			{
			case 0x00: // addr
				break;
			case 0x01: // ry
				set_memory_64(operand1, get_register(operand2));
				break;
			case 0x02: // imm
				set_memory_64(operand1, operand2);
				break;
			case 0x03: // [addr]
				set_memory_64(operand1, get_memory_64(operand2));
				break;
			case 0x04:// [rx]
				set_memory_64(operand1, get_memory_64(get_register(operand2)));
				break;
			}
			break;
		case 0x01: // mov rx
			switch (op2_marker)
			{
			case 0x00: case 0x02: // addr || imm
				set_register(operand1, operand2);
				break;
			case 0x01: // ry
				set_register(operand1, get_register(operand2));
				break;
			case 0x03: // [addr]
				set_register(operand1, get_memory_64(operand2));
				break;
			case 0x04: // [ry]
				set_register(operand1, get_memory_64(get_register(operand2)));
				break;
			}
			break;
		case 0x03: // mov [addr]
			switch (op2_marker)
			{
			case 0x00: // addr
				break;
			case 0x02: // imm
				set_memory_64(get_memory_64(operand1), operand2);
				break;
			case 0x01: // ry
				set_memory_64(get_memory_64(operand1), get_register(operand2));
				break;
			case 0x03: // [addr]
				set_memory_64(get_memory_64(operand1), get_memory_64(operand2));
				break;
			case 0x04: // [ry]
				set_memory_64(get_memory_64(operand1), get_memory_64(get_register(operand2)));
				break;
			}
			break;
		case 0x04: // mov [rx]
			switch (op2_marker)
			{
			case 0x00: // addr
				break;
			case 0x02: // imm
				set_memory_64(get_register(operand1), operand2);
				break;
			case 0x01: // ry
				set_memory_64(get_register(operand1), get_register(operand2));
				break;
			case 0x03: // [addr]
				set_memory_64(get_register(operand1), get_memory_64(operand2));
				break;
			case 0x04: // [ry]
				set_memory_64(get_register(operand1), get_memory_64(get_register(operand2)));
				break;
			}
			break;
		}
		break;

	case 0x0a: // jmp
		switch (op1_marker)
		{
		case 0x00: // addr
			program_counter = operand1;
			break;
		case 0x03: // [addr]
			program_counter = get_memory_64(operand1);
			break;
		case 0x04: // [rx]
			program_counter = get_memory_64(get_register(operand1));
			break;
		}
		break;
	case 0x0b: // jump if equal
		if (get_register(0xf4) == 0)
			switch (op1_marker)
			{
			case 0x00: // addr
				program_counter = operand1;
				break;
			case 0x03: // [addr]
				program_counter = get_memory_64(operand1);
				break;
			case 0x04: // [rx]
				program_counter = get_memory_64(get_register(operand1));
				break;
			}
		break;
	case 0x0c: // jump if not equal
		if (get_register(0xf4) > 0)
			switch (op1_marker)
			{
			case 0x00: // addr
				program_counter = operand1;
				break;
			case 0x03: // [addr]
				program_counter = get_memory_64(operand1);
				break;
			case 0x04: // [rx]
				program_counter = get_memory_64(get_register(operand1));
				break;
			}
		break;
	case 0x0d: // jump if greater or equal
		if (get_register(0xf4) < 2)
			switch (op1_marker)
			{
			case 0x00: // addr
				program_counter = operand1;
				break;
			case 0x03: // [addr]
				program_counter = get_memory_64(operand1);
				break;
			case 0x04: // [rx]
				program_counter = get_memory_64(get_register(operand1));
				break;
			}
		break;
	case 0x0e: // jump if less
		if (get_register(0xf4) == 2)
			switch (op1_marker)
			{
			case 0x00: // addr
				program_counter = operand1;
				break;
			case 0x03: // [addr]
				program_counter = get_memory_64(operand1);
				break;
			case 0x04: // [rx]
				program_counter = get_memory_64(get_register(operand1));
				break;
			}
		break;
	case 0x0f: // compare 2 values
		if (op1_marker == 0x01)
			value1 = get_register(operand1);
		else
			value1 = operand1;
		if (op2_marker == 0x01)
			value2 = get_register(operand2);
		else
			value2 = operand2;
		if (op1_marker == 0x01 || op1_marker == 0x02 ||
			op2_marker == 0x01 || op2_marker == 0x02)
		{
			// r3 set to 0 if op1 and op2 are equal
			if (value1 == value2)
				set_register(0xf4, 0); 
			// r3 set to 1 if op1 is greater than op2
			else if (value1 > value2)
				set_register(0xf4, 1);
			// r3 set to 1 if op1 is less than op2
			else if (value1 < value2)
				set_register(0xf4, 2);
		}
		break;

	case 0x10:
		in(operand1);
		break;

	case 0x11:
		out(operand1);
		break;

	case 0x12:
		halted = true;
		break;
	}
}
