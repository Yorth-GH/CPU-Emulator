#include "includes.h"

uint64_t CPU::add(uint64_t a, uint64_t b)
{
	return CPU::registers[a - 240] + CPU::registers[b - 240];
}

uint64_t CPU::sub(uint64_t a, uint64_t b)
{
	return CPU::registers[a - 240] - CPU::registers[b - 240];
}

uint64_t CPU::mul(uint64_t a, uint64_t b)
{
	return CPU::registers[a - 240] * CPU::registers[b - 240];
}

uint64_t CPU::div(uint64_t a, uint64_t b)
{
	return CPU::registers[a - 240] * CPU::registers[b - 240];
}

uint64_t CPU::BINand(uint64_t a, uint64_t b)
{
	return CPU::registers[a - 240] & CPU::registers[b - 240];
}

uint64_t CPU::BINor(uint64_t a, uint64_t b)
{
	return CPU::registers[a - 240] | CPU::registers[b - 240];
}

uint64_t CPU::BINnot(uint64_t a)
{
	return ~CPU::registers[a - 240];
}

uint64_t CPU::BINxor(uint64_t a, uint64_t b)
{
	return CPU::registers[a - 240] * CPU::registers[b - 240];
}