#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
    operand_read(&opr_dest);
	if (opr_src.data_size < opr_dest.data_size) // do sign extend
    {
        opr_src.val = sign_ext(opr_src.val & (0xFFFFFFFF >> (32 - opr_src.data_size)), opr_src.data_size);
        opr_src.val = opr_src.val & (0xFFFFFFFF >> (32 - opr_dest.data_size));
    }
    opr_dest.val = alu_and(opr_src.val, opr_dest.val, opr_dest.data_size);
	cpu.eflags.CF = cpu.eflags.OF = 0;
}

make_instr_impl_2op(test, r, rm, b)
make_instr_impl_2op(test, r, rm, v)
make_instr_impl_2op(test, i, a, b)
make_instr_impl_2op(test, i, a, v)
make_instr_impl_2op(test, i, rm, b)
make_instr_impl_2op(test, i, rm, v)
