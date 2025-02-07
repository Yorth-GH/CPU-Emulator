#pragma once
#include "includes.h"

/*
* instruction set:
* 0x01 add					rx ry
* 0x02 sub					rx ry
* 0x03 mul					rx ry
* 0x04 div					rx ry
* 0x05 and					rx ry
* 0x06 or					rx ry
* 0x07 not					rx
* 0x08 xor					rx ry
* 0x09 mov					( rx,addr,[rx],[addr] ) ( ry,addr,[ry],[addr], imm )
* 0x0a jmp					( addr, [addr], [rx] )
* 0x0b je					( addr, [addr], [rx] )
* 0x0c jne					( addr, [addr], [rx] )
* 0x0d jge					( addr, [addr], [rx] )
* 0x0e jl					( addr, [addr], [rx] )
* 0x0f cmp					( rx, imm ) ( ry, imm )
* 0x10 in					rx
* 0x11 out					rx
* 0x12 halt
*
* registers:
* 0xf0						r0
* 0xf1						r1
* 0xf2						r2
* 0xf3						r3
*
* flags:
* 0x00 - address			addr
* 0x01 - register			rx
* 0x02 - immediate			imm
* 0x03 - indirect address	[addr]
* 0x04 - indirect register	[rx]
*
* cmp values:
* 0	- equal
* 1 - greater than
* 2 - less than
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
	uint64_t add(uint64_t a, uint64_t b);		
	uint64_t sub(uint64_t a, uint64_t b);		
	uint64_t mul(uint64_t a, uint64_t b);		
	uint64_t div(uint64_t a, uint64_t b);		
	uint64_t BINand(uint64_t a, uint64_t b);	
	uint64_t BINor(uint64_t a, uint64_t b);		
	uint64_t BINnot(uint64_t a);				
	uint64_t BINxor(uint64_t a, uint64_t b);	

	// CU
	bool get_halted() const;
	uint8_t fetch();
	uint64_t fetch_64();
	void decode();
	void execute();
	// cmp stores result in r3
	// SAVE THE VALUE IN r3 BEFORE CALLING CMP
	
	// call cmp before conditional jumps
	// not running cmp can result in unwanted behavior

	// IO
	bool load_operand(std::string operand, uint64_t& temp_address);
	bool load_program(std::string file);
	void in(uint64_t reg);
	void out(uint64_t reg);
};
