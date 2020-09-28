#include "cpu.h"

void CPU::load_program(std::vector<uchar>& program) {
	if (program.size() > ADDRESS_SPACE) {
		fprintf(stderr, "[!] error: program too big");
		abort();
	}

	std::copy(program.begin(), program.end(), state.memory.begin());

	return;
}

void CPU::fetch() {
	currInsn = state.read_mem(state.get_pc(), false);
	return;
}

void CPU::decode() {
	TYPE op = static_cast<TYPE>(currInsn >> 13);
	switch (op) {
		// SINGLE_OP, COND, and DOUBLE_OP cannot be
		// assignment-initialized because of C++
		// scoping rules and control flow
		case TYPE::SINGLE:
			SINGLE_OP sop;
			sop = static_cast<SINGLE_OP>((currInsn >> 7) & 0x7);
			currDecInsn = std::make_unique<SingleOp>(currInsn, sop);
			break;
		case TYPE::JUMP:
			COND cond;
			cond = static_cast<COND>((currInsn >> 10) & 0x7);
			currDecInsn = std::make_unique<Jump>(currInsn, cond);
			break;
		default:
			DOUBLE_OP dop;
			dop = static_cast<DOUBLE_OP>((currInsn >> 12) & 0xF);
			currDecInsn = std::make_unique<DoubleOp>(currInsn, dop);
			break;
	}
	return;
}

void CPU::execute() {
	currDecInsn->execute(state);
	uint16_t newpc = state.get_pc() + (currDecInsn->length / 8);
	state.set_pc(newpc);
	return;
}

void CPU::step() {
	fetch();
	decode();
	execute();
	return;
}

// extremely rough implementation, this will
// probably break when it throws some exception.
// This function only exists right now for
// testing purposes.
void CPU::run() {
	while (true) {
		step();
	}
	return;
}