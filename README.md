# CPU Emulator
**Computer Architecture Project**\
\
This CPU Emulator was made as part of my Computer Architecture course at Faculty of Electrical Engineering. 

## The emulator features:
- 4 64 bit General Purpose Registers
- 64 bit Address Space
- 18 instructions
- Program Counter
- Parsing assembler code into bytecode

## Instruction set: 
 FORMAT:\
`operation operand1 operand2`\
\
Operands are marked within brackets if multiple types of access are available. (ex. direct and indirect addressing)\
\
`rx - direct addressing register x`\
`addrx - direct addressing address x`\
`imm - immediate value`\
`[rx] - indirect addressing register x`\
`[addrx] - indirect addressing address x`\

- `add rx ry`<br> adds values from registers `rx` and `ry`, saves final value into `rx`
- `sub rx ry`<br> subtracts values from registers `rx` and `ry`, saves final value into `rx`
- `mul rx ry`<br> multiplies values from registers `rx` and `ry`, saves final value into `rx`
- `div rx ry`<br> divides values from registers `rx` and `ry`, saves final value into `rx`
- `and rx ry`<br> ANDs values from registers `rx` and `ry`, saves final value into `rx`
- `or rx ry`<br> ORs values from registers `rx` and `ry`, saves final value into `rx`
- `not rx`<br> NOTs values from register `rx`, saves final value into `rx`
- `xor rx ry`<br> XORs values from registers `rx` and `ry`, saves final value into `rx`
- `mov ( rx, addrx, [rx], [addrx] ) ( ry, addry, [ry], [addry], imm )`<br> moves values from `ry`, `addry`, address at `ry` and address at `addry` into `rx`, `addrx`, address at `rx` and address at `addrx`
- `jmp ( addr, [addr], [rx] )`<br> jumps to given address
- `cmp ( rx, imm ) ( ry, imm )`<br> compares `rx/imm` with `ry/imm`, saves final value into `r3`
- `je/jne/jge/jl ( addr, [addr], [rx] )`<br> jumps to given address if condition is met (is equal to / not equal to/ greater or equal / less than) from `r3`
- `in rx`<br> allows user to input a value from keyboard into register `rx`
- `out rx`<br> prints the value of register `rx`
- `halt`<br> stops the execution of CPU
