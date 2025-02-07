#include "includes.h"

void main() 
{
	CPU cpu;

	if (!cpu.load_program("program.txt"))
	{
		std::cout << "ERROR ??LOADING PROGRAM FAILED?? ERROR" << std::endl;
		return;
	}

	::testing::InitGoogleTest();	
	RUN_ALL_TESTS();

	while (!cpu.get_halted())
	{
		cpu.decode();
		cpu.execute();
	}
	std::cout << "Finished";
}