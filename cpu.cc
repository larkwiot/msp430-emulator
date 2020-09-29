#include "cpu.h"

void CPU::load_program(std::vector<uchar>& program) {
	if (program.size() > ADDRESS_SPACE) {
		ERR("program too big");
		abort();
	}

	std::copy(program.begin(), program.end(), state.memory.begin() + 0xC000);

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
			currDecInsn = std::make_unique<SingleOp>(currInsn);
			break;
		case TYPE::JUMP:
			currDecInsn = std::make_unique<Jump>(currInsn);
			break;
		default:
			currDecInsn = std::make_unique<DoubleOp>(currInsn);
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
	std::cout << get_string();
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

std::string CPU::get_string() {
	std::string s{};
	s += "[CPU] currInsn = " + std::to_string(currInsn) + "\n";
	s += "[CPU] currDecInsn:\n";
	s += currDecInsn->get_string() + "\n";
	s += state.get_string() + "\n";
	return s;
}