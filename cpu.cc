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
	// false because we always read the current instruction
	// as a word, never in byte mode
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
	std::cout << "[*] PC = " << int_to_hexstr(state.get_pc()) << "\n";

	fetch();
	std::cout << "[*] fetched: " << int_to_bytes_str(currInsn) << "\n";

	decode();
	
	execute();
	std::cout << "[*] executing: " << currDecInsn->get_string() << "\n";

	std::cout << "\n";
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
	s = "[CPU] currInsn = " + int_to_hexstr(currInsn) + "\n" +
			"[CPU] currDecInsn:\n";
	if (currDecInsn != nullptr) {
		s += currDecInsn->get_string();
	}
	s += state.get_string();
	return s;
}