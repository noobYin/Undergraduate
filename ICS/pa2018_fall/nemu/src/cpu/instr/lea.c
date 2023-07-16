#include "cpu/instr.h"

make_instr_func(lea) 
{
    OPERAND r, rm;
    r.data_size = data_size;
	rm.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    rm.sreg = SREG_SS;
    //operand_read(&rm);
    r.val = rm.addr;
    operand_write(&r);

    print_asm_2("lea", data_size == 8 ? "b" : (data_size == 16 ? "w" : "l"), len, &rm, &r);
    return len;
}
