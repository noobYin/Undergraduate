#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	opr_dest.val = opr_src.val;
	operand_write(&opr_dest);
}

make_instr_impl_2op(mov, r, rm, b)
make_instr_impl_2op(mov, r, rm, v)
make_instr_impl_2op(mov, rm, r, b)
make_instr_impl_2op(mov, rm, r, v)
make_instr_impl_2op(mov, i, rm, b)
make_instr_impl_2op(mov, i, rm, v)
make_instr_impl_2op(mov, i, r, b)
make_instr_impl_2op(mov, i, r, v)
make_instr_impl_2op(mov, a, o, b)
make_instr_impl_2op(mov, a, o, v)
make_instr_impl_2op(mov, o, a, b)
make_instr_impl_2op(mov, o, a, v)

extern void load_sreg(uint8_t sreg);
make_instr_func(mov_rm2s_w)
{
	OPERAND rm, s;
	uint8_t sreg;
	int len = 1;

	len += modrm_opcode_rm(eip + 1, &sreg, &rm);
	rm.data_size = 16;
	operand_read(&rm);

	s.type = OPR_SREG;
	s.data_size = 16;
	s.addr = sreg;
	s.val = rm.val & 0xffff;
	operand_write(&s);
	//printf("sreg is %u, s val is %u\n", sreg, s.val);
	load_sreg(sreg);
	print_asm_2("mov_rm2s", "w", 2, &rm, &s);
	return len;
}

make_instr_func(mov_c2r_l)
{
	OPERAND c, r;
	int len = 1; // opcode 2 bytes
	uint8_t caddr;

	len += modrm_opcode_rm(eip + 1, &caddr, &r);

	c.type = OPR_CREG;
	c.data_size = 32;
	c.addr = caddr;
	operand_read(&c);

	r.type = OPR_REG;
	r.data_size = 32;

	r.val = c.val;
	operand_write(&r);
	print_asm_2("mov_c2r", "l", 3, &c, &r);
	//printf("len = %d\n", len);
	return len;
}

make_instr_func(mov_r2c_l)
{
	OPERAND c, r;
	int len = 1; // opcode 2 bytes
	uint8_t caddr;
	len += modrm_opcode_rm(eip + 1, &caddr, &r);

	r.type = OPR_REG;
	r.data_size = 32;
	operand_read(&r);

	c.val = r.val;
	c.type = OPR_CREG;
	c.data_size = 32;
	c.addr = caddr;
	operand_write(&c);
	print_asm_2("mov_r2c", "l", 3, &r, &c);
	//printf("len = %d\n", len);
	return len;
}

/*
make_instr_func(mov_r2rm_b) {
	src.data_size = 8;
	dest.data_size = 8;

	len += modrm_r_rm(eip + 1, &src, &dest);
	
	OPERAND r, rm;
	r.data_size = 8;
	rm.data_size = 8;
	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);
	//operand_read(&r);
	//rm.val = r.val;
	//operand_write(&rm);
	execute();
	return len;
}*/



/*make_instr_func(mov_r2rm_v) {
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = data_size;
	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);
	//printf("mov_r2rm rm addr = 0x%08x\n", rm.addr);
	//operand_read(&r);
	//rm.val = r.val;
	//operand_write(&rm);
	execute(&r, &rm);
	return len;
}

make_instr_func(mov_rm2r_b) {
	OPERAND r, rm;
	r.data_size = 8;
	rm.data_size = 8;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);

	//operand_read(&rm);
	//r.val = rm.val;
	//operand_write(&r);
	execute(&rm, &r);
	return len;
}

make_instr_func(mov_rm2r_v) {
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = data_size;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);

	//operand_read(&rm);
	//r.val = rm.val;
	//operand_write(&r);
	execute(&rm, &r);
	return len;
}

make_instr_func(mov_i2rm_b) {
	OPERAND rm, imm;
	rm.data_size = 8;
	imm.data_size = 8;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_rm(eip + 1, &rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	// printf("addr imm %08x, len=%d\n", imm.addr, len);
	//operand_read(&imm);
	//rm.val = imm.val;
	//operand_write(&rm);
	execute(&imm, &rm);
	return len + 1; 
}

make_instr_func(mov_i2rm_v) {
	OPERAND rm, imm;
	rm.data_size = data_size;
	imm.data_size = data_size;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_rm(eip + 1, &rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	imm.data_size = data_size;
	// printf("addr imm %08x, len=%d\n", imm.addr, len);

	//operand_read(&imm);
	//rm.val = imm.val;
	//operand_write(&rm);
	execute(&imm, &rm);
	return len + data_size / 8;
}
make_instr_func(mov_i2r_b) {
	OPERAND r, imm;
	r.data_size = 8;
	r.type = OPR_REG;
	r.addr = opcode & 0x7;

	imm.type = OPR_IMM;
	imm.addr = eip + 1;
	imm.data_size = 8;

	//operand_read(&imm);
	//r.val = imm.val;
	//operand_write(&r);

	execute(&imm, &r);
	return 2;
}

make_instr_func(mov_i2r_v) {
	OPERAND r, imm;
	r.data_size = data_size;
	r.type = OPR_REG;
	r.addr = opcode & 0x7;

	imm.type = OPR_IMM;
	imm.addr = eip + 1;
	imm.data_size = data_size;

	//operand_read(&imm);
	//r.val = imm.val;
	//operand_write(&r);

	execute(&imm, &r);
	return 1 + data_size / 8;
}

make_instr_func(mov_off2a_b) {
	OPERAND r, m;
	r.data_size = 8;
	r.type = OPR_REG;
	r.addr = REG_AL;

	m.type = OPR_MEM;
	m.sreg = SREG_DS;
	m.addr = instr_fetch(eip + 1, 4);
	m.data_size = 8;

	operand_read(&m);
	r.val = m.val;
	operand_write(&r);
	return 5;
}


make_instr_func(mov_off2a_v) {
	OPERAND r, m;
	r.data_size = data_size;
	r.type = OPR_REG;
	r.addr = REG_AX;
	
	m.type = OPR_MEM;
	m.sreg = SREG_DS;
	m.addr = instr_fetch(eip + 1, 4);
	m.data_size = data_size;

	operand_read(&m);
	r.val = m.val;
	operand_write(&r);
	return 5;
}

make_instr_func(mov_a2off_b) {
	OPERAND r, m;
	r.data_size = 8;
	r.type = OPR_REG;
	r.addr = REG_AL;

	m.type = OPR_MEM;
	m.sreg = SREG_DS;
	m.addr = instr_fetch(eip + 1, 4);
	m.data_size = 8;

	operand_read(&r);
	m.val = r.val;
	operand_write(&m);
	return 5;
}

make_instr_func(mov_a2off_v) {
	OPERAND r, m;
	r.data_size = data_size;
	r.type = OPR_REG;
	r.addr = REG_AX;

	m.type = OPR_MEM;
	m.sreg = SREG_DS;
	m.addr = instr_fetch(eip + 1, 4);
	m.data_size = data_size;

	operand_read(&r);
	m.val = r.val;
	operand_write(&m);
	return 5;
}

*/
make_instr_func(mov_zrm82r_v) {
	int len = 1;
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = 8;
	len += modrm_r_rm(eip + 1, &r, &rm);
	
	operand_read(&rm);
	r.val = rm.val;
	operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
	return len;
}

make_instr_func(mov_zrm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);

        operand_read(&rm);
        r.val = rm.val;
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm82r_v) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = data_size;
        rm.data_size = 8;
        len += modrm_r_rm(eip + 1, &r, &rm);
        
	operand_read(&rm);
        r.val = sign_ext(rm.val, 8);
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);
        operand_read(&rm);
        r.val = sign_ext(rm.val, 16);
        operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
        return len;
}
