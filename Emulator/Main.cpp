#include "includes.h"

void main() 
{
	CPU cpu;

	if (!cpu.load_program("program.txt"))
	{
		std::cout << "ERROR ??LOADING PROGRAM FAILED?? ERROR" << std::endl;
		return;
	}

	// RUN
	while (!cpu.get_halted())
	{
		cpu.decode();
		cpu.execute();
	}
}