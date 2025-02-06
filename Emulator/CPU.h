#pragma once

#include "includes.h"

/*
* instruction set:
* 0x01 add
* 0x02 sub
* 0x03 mul
* 0x04 div
* 0x05 and
* 0x06 or
* 0x07 not
* 0x08 xor
* 0x09 mov
* 0x0a jmp
* 0x0b je
* 0x0c jne
* 0x0d jge
* 0x0e jl
* 0x0f cmp
* 0x10 in
* 0x11 out
* 0x12 halt
* 
* registers:
* 0xf0 r0
* 0xf1 r1
* 0xf2 r2
* 0xf3 r3
* 
* mov flags
* 0x00 - address
* 0x01 - register
* 0x02 - immediate
* 0x03 - indirect address
* 0x04 - indirect register
*/ 


class CPU 
{
private:
	uint64_t registers[4] = { 0 };
	uint64_t program_counter = 0;
	std::unordered_map<uint64_t, uint8_t> memory;

	std::unordered_map<std::string, uint8_t> opcode_map = 
	{
		{"add", 0x01}, {"sub", 0x02}, {"mul", 0x03}, {"div", 0x04},
		{"and", 0x05}, {"or", 0x06}, {"not", 0x07}, {"xor", 0x08},
		{"mov", 0x09}, {"jmp", 0x0a}, {"je", 0x0b}, {"jne", 0x0c},
		{"jge", 0x0d}, {"jl", 0x0e}, {"cmp", 0x0f}, {"in", 0x10},
		{"out", 0x11}, {"halt", 0x12}
	};

	std::unordered_map<std::string, uint8_t> register_map = 
	{
	   {"r0", 0xf0}, {"r1", 0xf1}, {"r2", 0xf2}, {"r3", 0xf3}
	};

	// for decoding and executing
	uint8_t opcode;
	uint64_t operand1, operand2;
	uint8_t op1_marker, op2_marker;
	bool halted = false;
public:
	// CPU
	uint64_t get_register(uint64_t reg);
	bool set_register(uint64_t reg, uint64_t data);
	bool set_memory(uint64_t address, uint8_t data);
	bool set_memory_64(uint64_t address, uint64_t data);
	uint8_t get_memory(uint64_t address);
	uint64_t get_memory_64(uint64_t address);

	// ALU
	uint64_t add(uint64_t a, uint64_t b);		// add rx ry, saves to rx
	uint64_t sub(uint64_t a, uint64_t b);		// sub rx ry, saves to rx
	uint64_t mul(uint64_t a, uint64_t b);		// mul rx ry, saves to rx
	uint64_t div(uint64_t a, uint64_t b);		// div rx ry, saves to rx
	uint64_t BINand(uint64_t a, uint64_t b);	// and rx ry, saves to rx
	uint64_t BINor(uint64_t a, uint64_t b);		// or rx ry, saves to rx
	uint64_t BINnot(uint64_t a);				// not rx, saves to rx
	uint64_t BINxor(uint64_t a, uint64_t b);	// xor rx ry, saves to rx

	// CU
	bool get_halted();
	uint8_t fetch();
	uint64_t fetch_64();
	void decode();
	void execute();

	// IO
	bool load_program(std::string file);
	void in(uint64_t reg);
	void out(uint64_t reg);
};