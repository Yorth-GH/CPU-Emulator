#include "includes.h"

uint64_t CPU::add(uint64_t a, uint64_t b)
{
	return get_register(a) + get_register(b);
}

uint64_t CPU::sub(uint64_t a, uint64_t b)
{
	return get_register(a) - get_register(b);
}

uint64_t CPU::mul(uint64_t a, uint64_t b)
{
	return get_register(a) * get_register(b);
}

uint64_t CPU::div(uint64_t a, uint64_t b)
{
	return get_register(a) / get_register(b);
}

uint64_t CPU::BINand(uint64_t a, uint64_t b)
{
	return get_register(a) & get_register(b);
}

uint64_t CPU::BINor(uint64_t a, uint64_t b)
{
	return get_register(a) | get_register(b);
}

uint64_t CPU::BINnot(uint64_t a)
{
	return ~get_register(a);
}

uint64_t CPU::BINxor(uint64_t a, uint64_t b)
{
	return get_register(a) ^ get_register(b);
}
