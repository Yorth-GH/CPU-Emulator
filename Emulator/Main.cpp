#include "includes.h"

void main() 
{
	CPU cpu;

	cpu.load_program("program.txt");

	// RUN
	while (!cpu.get_halted())
	{
		cpu.decode();
		cpu.execute();
	}
}