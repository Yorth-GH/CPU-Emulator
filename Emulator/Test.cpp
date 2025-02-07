#include "includes.h"

uint64_t temp_address = 0;

void CPU::load_line(std::string line)
{
	std::string aline;

		std::istringstream iss(line);
		std::string instruction, operand1, operand2;
		iss >> instruction >> operand1 >> operand2;

		if (opcode_map.count(instruction))
		{
			set_memory(temp_address++, opcode_map.at(instruction));

			if (opcode_map.at(instruction) == 0x12) // halt, no operands
				return;

			if (!load_operand(operand1, temp_address))
				return;

			if (opcode_map.at(instruction) == 0x07 || opcode_map.at(instruction) == 0x10 || opcode_map.at(instruction) == 0x11 ||
				opcode_map.at(instruction) == 0x0a || opcode_map.at(instruction) == 0x0b || opcode_map.at(instruction) == 0x0c ||
				opcode_map.at(instruction) == 0x0d || opcode_map.at(instruction) == 0x0e) // single operand instructions
				return;

			if (!load_operand(operand2, temp_address))
				return;
		}
		else
		{
			std::cout << "ERROR ??INVALID INSTRUCTION?? ERROR" << std::endl << "Instruction: " << instruction << std::endl;
			return;
		}
}

class CPU_TEST : public ::testing::Test
{
protected:
	CPU testCPU;

	void SetUp() override
	{
		testCPU.reset();
		temp_address = 0;
	}
};

//done
TEST_F(CPU_TEST, mov_indirreg_reg)
{
	testCPU.load_line("mov r1 0000000000000100");
	testCPU.load_line("mov r0 10");
	testCPU.load_line("mov [r1] r0");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_memory_64(0x100), 0x10);
}

TEST_F(CPU_TEST, mov_indirreg_imm)
{
	testCPU.load_line("mov r0 0000000000000100");
	testCPU.load_line("mov [r0] 5");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_memory_64(0x100), 0x05);
}

TEST_F(CPU_TEST, mov_indirreg_indirreg)
{
	testCPU.load_line("mov 0000000000000050 20");
	testCPU.load_line("mov r0 0000000000000050");
	testCPU.load_line("mov r1 0000000000000100");
	testCPU.load_line("mov [r1] [r0]");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_memory_64(0x100), 0x20);
}

TEST_F(CPU_TEST, mov_indirreg_indiradd)
{
	testCPU.load_line("mov 0000000000000100 20");
	testCPU.load_line("mov r1 0000000000000050");
	testCPU.load_line("mov [r1] [0000000000000100]");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_memory_64(0x50), 0x20);
}


//done
TEST_F(CPU_TEST, mov_indiradd_reg)
{
	testCPU.load_line("mov r0 10");
	testCPU.load_line("mov r1 0000000000000050");
	testCPU.load_line("mov 0000000000000100 r1");
	testCPU.load_line("mov [0000000000000100] r0");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_memory_64(0x50), 0x10);
}

TEST_F(CPU_TEST, mov_indiradd_imm)
{
	testCPU.load_line("mov r1 0000000000000050");
	testCPU.load_line("mov 0000000000000100 r1");
	testCPU.load_line("mov [0000000000000100] 5");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_memory_64(0x50), 0x5);
}

TEST_F(CPU_TEST, mov_indiradd_indirreg)
{
	testCPU.load_line("mov 0000000000000100 20");
	testCPU.load_line("mov r1 [0000000000000100]");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf1), 0x20);
}

TEST_F(CPU_TEST, mov_indiradd_indiradd)
{
	testCPU.load_line("mov 0000000000000100 20");
	testCPU.load_line("mov r1 [0000000000000100]");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf1), 0x20);
}

//done
TEST_F(CPU_TEST, mov_reg_reg)
{
	testCPU.load_line("mov r1 10");
	testCPU.load_line("mov r0 1");
	testCPU.load_line("mov r0 r1");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf0), 0x10);
}

TEST_F(CPU_TEST, mov_reg_add)
{
	testCPU.load_line("mov r0 0000000000000010");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf0), 0x10);
}

TEST_F(CPU_TEST, mov_reg_imm)
{
	testCPU.load_line("mov r0 5");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf0), 0x05);
}

TEST_F(CPU_TEST, mov_reg_indirreg)
{
	testCPU.load_line("mov 0000000000000100 20");
	testCPU.load_line("mov r0 0000000000000100");
	testCPU.load_line("mov r1 [r0]");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf1), 0x20);
}

TEST_F(CPU_TEST, mov_reg_indiradd)
{
	testCPU.load_line("mov 0000000000000100 20");
	testCPU.load_line("mov r1 [0000000000000100]");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf1), 0x20);
}


//done
TEST_F(CPU_TEST, mov_add_reg)
{
	testCPU.load_line("mov r1 50");
	testCPU.load_line("mov 0000000000000100 r1");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_memory_64(0x100), 0x50);
}

TEST_F(CPU_TEST, mov_add_imm)
{
	testCPU.load_line("mov 0000000000000100 50");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_memory_64(0x100), 0x50);
}

TEST_F(CPU_TEST, mov_add_indirreg)
{
	testCPU.load_line("mov r1 0000000000000050");
	testCPU.load_line("mov 0000000000000050 20");
	testCPU.load_line("mov 0000000000000100 [r1]");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_memory_64(0x100), 0x20);
}

TEST_F(CPU_TEST, mov_add_indiradd)
{
	testCPU.load_line("mov r0 50");
	testCPU.load_line("mov 0000000000000500 r0");
	testCPU.load_line("mov 0000000000000100 [0000000000000500]");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_memory_64(0x100), 0x50);
}


//done
TEST_F(CPU_TEST, add_reg_reg)
{
	testCPU.load_line("mov r0 50");
	testCPU.load_line("mov r1 20");
	testCPU.load_line("add r0 r1");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf0), 0x70);
}

TEST_F(CPU_TEST, sub_reg_reg)
{
	testCPU.load_line("mov r0 50");
	testCPU.load_line("mov r1 20");
	testCPU.load_line("sub r0 r1");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf0), 0x30);
}

TEST_F(CPU_TEST, mul_reg_reg)
{
	testCPU.load_line("mov r0 50");
	testCPU.load_line("mov r1 20");
	testCPU.load_line("mul r0 r1");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf0), 0xA00);
}

TEST_F(CPU_TEST, div_reg_reg)
{
	testCPU.load_line("mov r0 60");
	testCPU.load_line("mov r1 20");
	testCPU.load_line("div r0 r1");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf0), 0x3);
}


//done
TEST_F(CPU_TEST, and_reg_reg)
{
	testCPU.load_line("mov r0 50");
	testCPU.load_line("mov r1 20");
	testCPU.load_line("and r0 r1");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf0), 0x00);
}

TEST_F(CPU_TEST, xor_reg_reg)
{
	testCPU.load_line("mov r0 50");
	testCPU.load_line("mov r1 20");
	testCPU.load_line("xor r0 r1");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf0), 0x70);
}

TEST_F(CPU_TEST, or_reg_reg)
{
	testCPU.load_line("mov r0 50");
	testCPU.load_line("mov r1 20");
	testCPU.load_line("or r0 r1");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf0), 0x70);
}

TEST_F(CPU_TEST, not_reg)
{
	testCPU.load_line("mov r0 50");
	testCPU.load_line("not r0");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf0), ~0x50);
}


//done
TEST_F(CPU_TEST, in_reg)
{
	std::cout << "IN test! Enter 1!" << std::endl;
	testCPU.load_line("in r0");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf0), 0x1);
}

TEST_F(CPU_TEST, out_reg)
{
	testCPU.load_line("mov r0 50");
	testCPU.load_line("out r0");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf0), 0x50);
}

TEST_F(CPU_TEST, halted)
{
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_halted(), true);
}


//done
TEST_F(CPU_TEST, jump_add)
{
	testCPU.load_line("jmp 0000000000000090");
	testCPU.set_memory_64(0x90, 0x12);

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_pc(), 0x91);
}

TEST_F(CPU_TEST, jump_indiradd)
{
	testCPU.load_line("mov r0 0000000000000050");
	testCPU.load_line("mov 0000000000000090 r0");
	testCPU.load_line("jmp [0000000000000090]");
	testCPU.set_memory_64(0x50, 0x12);

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_pc(), 0x51);
}

TEST_F(CPU_TEST, jump_indirreg)
{
	testCPU.load_line("mov r0 0000000000000090");
	testCPU.load_line("jmp [r0]");
	testCPU.set_memory_64(0x90, 0x12);

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_pc(), 0x91);
}


//done
TEST_F(CPU_TEST, je_add)
{
	testCPU.set_register(4, 0);
	testCPU.load_line("je 0000000000000090");
	testCPU.set_memory_64(0x90, 0x12);

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_pc(), 0x91);
}

TEST_F(CPU_TEST, je_indiradd)
{
	testCPU.set_register(4, 0);
	testCPU.load_line("mov r0 0000000000000050");
	testCPU.load_line("mov 0000000000000090 r0");
	testCPU.load_line("je [0000000000000090]");
	testCPU.set_memory_64(0x50, 0x12);

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_pc(), 0x51);
}

TEST_F(CPU_TEST, je_indirreg)
{
	testCPU.set_register(4, 0);
	testCPU.load_line("mov r0 0000000000000090");
	testCPU.load_line("je [r0]");
	testCPU.set_memory_64(0x90, 0x12);

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_pc(), 0x91);
}


//done 
TEST_F(CPU_TEST, jne_add)
{
	testCPU.set_register(4, 1);
	testCPU.load_line("jne 0000000000000090");
	testCPU.set_memory_64(0x90, 0x12);

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_pc(), 0x91);
}

TEST_F(CPU_TEST, jne_indiradd)
{
	testCPU.set_register(4, 2);
	testCPU.load_line("mov r0 0000000000000050");
	testCPU.load_line("mov 0000000000000090 r0");
	testCPU.load_line("jne [0000000000000090]");
	testCPU.set_memory_64(0x50, 0x12);

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_pc(), 0x51);
}

TEST_F(CPU_TEST, jne_indirreg)
{
	testCPU.set_register(4, 1);
	testCPU.load_line("mov r0 0000000000000090");
	testCPU.load_line("jne [r0]");
	testCPU.set_memory_64(0x90, 0x12);

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_pc(), 0x91);
}


//done
TEST_F(CPU_TEST, jge_add)
{
	testCPU.set_register(4, 1);
	testCPU.load_line("jge 0000000000000090");
	testCPU.set_memory_64(0x90, 0x12);

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_pc(), 0x91);
}

TEST_F(CPU_TEST, jge_indiradd)
{
	testCPU.set_register(4, 0);
	testCPU.load_line("mov r0 0000000000000050");
	testCPU.load_line("mov 0000000000000090 r0");
	testCPU.load_line("jge [0000000000000090]");
	testCPU.set_memory_64(0x50, 0x12);

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_pc(), 0x51);
}

TEST_F(CPU_TEST, jge_indirreg)
{
	testCPU.set_register(4, 1);
	testCPU.load_line("mov r0 0000000000000090");
	testCPU.load_line("jge [r0]");
	testCPU.set_memory_64(0x90, 0x12);

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_pc(), 0x91);
}


//done
TEST_F(CPU_TEST, jl_add)
{
	testCPU.set_register(4, 2);
	testCPU.load_line("jl 0000000000000090");
	testCPU.set_memory_64(0x90, 0x12);

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_pc(), 0x91);
}

TEST_F(CPU_TEST, jl_indiradd)
{
	testCPU.set_register(4, 2);
	testCPU.load_line("mov r0 0000000000000050");
	testCPU.load_line("mov 0000000000000090 r0");
	testCPU.load_line("jl [0000000000000090]");
	testCPU.set_memory_64(0x50, 0x12);

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_pc(), 0x51);
}

TEST_F(CPU_TEST, jl_indirreg)
{
	testCPU.set_register(4, 2);
	testCPU.load_line("mov r0 0000000000000090");
	testCPU.load_line("jl [r0]");
	testCPU.set_memory_64(0x90, 0x12);

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_pc(), 0x91);
}


//done
TEST_F(CPU_TEST, cmp_imm_imm)
{
	testCPU.load_line("cmp 5 10");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf4), 0x2);
}

TEST_F(CPU_TEST, cmp_reg_imm)
{
	testCPU.load_line("mov r0 10");
	testCPU.load_line("cmp r0 10");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf4), 0x0);
}

TEST_F(CPU_TEST, cmp_imm_reg)
{
	testCPU.load_line("mov r0 10");
	testCPU.load_line("cmp 5 r0");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf4), 0x2);
}

TEST_F(CPU_TEST, cmp_reg_reg)
{
	testCPU.load_line("mov r0 10");
	testCPU.load_line("mov r1 12");
	testCPU.load_line("cmp r1 r0");
	testCPU.load_line("halt");

	while (!testCPU.get_halted())
	{
		testCPU.decode();
		testCPU.execute();
	}
	EXPECT_EQ(testCPU.get_register(0xf4), 0x1);
}