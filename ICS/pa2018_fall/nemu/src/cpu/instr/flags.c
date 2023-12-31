#include "cpu/instr.h"

make_instr_func(cld) {
	cpu.eflags.DF = 0;
	print_asm_0("cld", "", 1);
	return 1;
}

make_instr_func(clc) {
    cpu.eflags.CF = 0;
	print_asm_0("clc", "", 1);
    return 1;
}

make_instr_func(sahf) {
	cpu.eflags.val = (cpu.eflags.val & 0xffff0000) | cpu.gpr[0]._8[1];
	print_asm_0("sahf", "", 1);
	return 1;
}

make_instr_func(cli)
{
	cpu.eflags.IF = 0;
	print_asm_0("cli", "", 1);
	return 1;
}

make_instr_func(sti)
{
	cpu.eflags.IF = 1;
	print_asm_0("sti", "", 1);
	return 1;
}

make_instr_func(std)
{
	cpu.eflags.DF = 1;
	print_asm_0("std", "", 1);
	return 1;
}

make_instr_func(stc)
{
	cpu.eflags.CF = 1;
	print_asm_0("stc", "", 1);
	return 1;
}