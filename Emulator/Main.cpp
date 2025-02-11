#include "includes.h"

void main() 
{
	//::testing::InitGoogleTest();
	//RUN_ALL_TESTS();

	CPU cpu;

	if (!cpu.load_program("program.txt"))
	{
		std::cout << "ERROR ??LOADING PROGRAM FAILED?? ERROR" << std::endl;
		return;
	}

	while (!cpu.get_halted())
	{
		cpu.decode();
		cpu.execute();
	}
	std::cout << "Finished";
}