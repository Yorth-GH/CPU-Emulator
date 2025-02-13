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
* 0xf4						flag
*
* markers:
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

#define page_size 4096

class CPU 
{
private:
	uint64_t registers[5] = { 0 };
	uint64_t program_counter = 0;
	
	std::unordered_map<uint64_t, uint8_t*> memory;

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
	   {"r0", 0xf0}, {"r1", 0xf1}, {"r2", 0xf2}, {"r3", 0xf3}, {"flag", 0xf4}
	};

	// for decoding and executing
	uint8_t opcode = 0;
	uint64_t operand1 = 0, operand2 = 0;
	uint8_t op1_marker = 0, op2_marker = 0;
	bool halted = false;

	// for cmp
	uint64_t value1 = 0;
	uint64_t value2 = 0;

public:
	~CPU();

	CPU() {};

	CPU& operator=(const CPU& other) 
	{
		if (this == &other) 
			return *this;

		for (auto& entry : memory)
			delete[] entry.second;

		memory.clear();

		program_counter = other.program_counter;
		std::copy(std::begin(other.registers), std::end(other.registers), std::begin(registers));
		opcode_map = other.opcode_map;
		register_map = other.register_map;
		opcode = other.opcode;
		operand1 = other.operand1;
		operand2 = other.operand2;
		op1_marker = other.op1_marker;
		op2_marker = other.op2_marker;
		halted = other.halted;
		value1 = other.value1;
		value2 = other.value2;

		for (auto& entry : other.memory) 
		{
			uint8_t* new_page = new uint8_t[page_size];
			std::memcpy(new_page, entry.second, page_size);
			memory[entry.first] = new_page;
		}
		return *this;
	}

	CPU& operator=(CPU&& other) noexcept
	{
		if (this == &other) return *this;

		for (auto& entry : memory)
			delete[] entry.second;

		memory.clear();

		program_counter = other.program_counter;
		std::copy(std::begin(other.registers), std::end(other.registers), std::begin(registers));
		opcode_map = std::move(other.opcode_map);
		register_map = std::move(other.register_map);
		opcode = other.opcode;
		operand1 = other.operand1;
		operand2 = other.operand2;
		op1_marker = other.op1_marker;
		op2_marker = other.op2_marker;
		halted = other.halted;
		value1 = other.value1;
		value2 = other.value2;
		memory = std::move(other.memory);

		other.program_counter = 0;
		other.halted = false;
		return *this;
	}

	CPU(const CPU& other) 
	{
		program_counter = other.program_counter;
		std::copy(std::begin(other.registers), std::end(other.registers), std::begin(registers));
		opcode_map = other.opcode_map;
		register_map = other.register_map;
		opcode = other.opcode;
		operand1 = other.operand1;
		operand2 = other.operand2;
		op1_marker = other.op1_marker;
		op2_marker = other.op2_marker;
		halted = other.halted;
		value1 = other.value1;
		value2 = other.value2;

		for (auto& entry : other.memory) 
		{
			uint8_t* new_page = new uint8_t[page_size];
			std::memcpy(new_page, entry.second, 4096);
			memory[entry.first] = new_page;
		}
	}

	CPU(CPU&& other) noexcept
	{
		program_counter = other.program_counter;
		std::copy(std::begin(other.registers), std::end(other.registers), std::begin(registers));
		opcode_map = std::move(other.opcode_map);
		register_map = std::move(other.register_map);
		opcode = other.opcode;
		operand1 = other.operand1;
		operand2 = other.operand2;
		op1_marker = other.op1_marker;
		op2_marker = other.op2_marker;
		halted = other.halted;
		value1 = other.value1;
		value2 = other.value2;
		memory = std::move(other.memory);

		other.program_counter = 0;
		other.halted = false;
	}

	// CPU
	uint64_t get_pc();
	uint64_t get_register(uint64_t reg);
	bool set_register(uint64_t reg, uint64_t data);
	bool set_memory(uint64_t address, uint8_t data);
	bool set_memory_64(uint64_t address, uint64_t data);
	uint8_t get_memory(uint64_t address);
	uint64_t get_memory_64(uint64_t address);
	void free_page(uint64_t address);
	void reset();

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

	// cmp stores result in flags
	// call cmp before conditional jumps
	// not running cmp can and probably will result in unwanted behavior

	// IO
	bool load_operand(std::string operand, uint64_t& temp_address);
	bool load_program(std::string file);
	void in(uint64_t reg);
	void out(uint64_t reg);

	// TESTING
	void load_line(std::string line);
};
