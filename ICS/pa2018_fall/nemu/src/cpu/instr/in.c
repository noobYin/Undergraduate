#include "cpu/instr.h"
#include "device/port_io.h"

make_instr_func(in_b)
{
    cpu.gpr[0]._8[0] = pio_read(cpu.gpr[2]._16, 1);
    return 1;
}

make_instr_func(in_v)
{
    if (data_size == 32)
        cpu.eax = pio_read(cpu.gpr[2]._16, 4);
    else if (data_size == 16)
        cpu.gpr[0]._16 = pio_read(cpu.gpr[2]._16, 2);
    return 1;
}