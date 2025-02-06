#include "includes.h"

int main() 
{
	CPU cpu;

	cpu.load_program("program.txt");

	while (!cpu.get_halted())
	{
		cpu.decode();
		cpu.execute();
	}

	std::cout << "Registar R0 je " << cpu.get_register(0xf0) << std::endl;
	std::cout << "Registar R1 je " << cpu.get_register(0xf1) << std::endl;
	std::cout << "Registar R2 je " << cpu.get_register(0xf2) << std::endl;
	std::cout << "Registar R3 je " << cpu.get_register(0xf3) << std::endl;

	return 0;
}