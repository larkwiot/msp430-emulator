#include "instructions.h"

uint8_t Operand::get_value(CPUState& state, int insnLength) {
	uint16_t constant;
	uint16_t regval;
	switch (addressMode) {
		case ADDRMODE::REGISTER:
			return state.get_reg(reg);

		case ADDRMODE::INDEXED:
		case ADDRMODE::SYMBOLIC:
			constant = state.peek(insnLength);
			regval = state.get_reg(reg);
			return state.read_mem(regval + constant, bw);

		case ADDRMODE::ABSOLUTE:
			return state.read_mem(state.peek(insnLength), bw);

		// intentional fall-through for clarity,
		// checked later in this method
		case ADDRMODE::INDIRECT_REGISTER:
			regval = state.get_reg(reg);
			return state.read_mem(regval, bw);
		case ADDRMODE::INDIRECT_AUTOINCREMENT:
			regval = state.get_reg(reg);
			state.registers.at(reg) += bw ? 1 : 2;
			return state.read_mem(regval, bw);

		case ADDRMODE::IMMEDIATE:
			return state.read_mem(insnLength, bw);

		case ADDRMODE::CONSTANT:
			return state.get_reg(REG::CG);
	}
}

void Operand::set_value(CPUState& state, int insnLength, uint16_t value) {
	value &= 0xFFFF;
	uint16_t constant;
	uint16_t regval;
	switch (addressMode) {
		case ADDRMODE::REGISTER:
			state.set_reg(reg, value);
			break;

		case ADDRMODE::INDEXED:
		case ADDRMODE::SYMBOLIC:
			constant = state.peek(insnLength);
			regval = state.get_reg(reg);
			state.write_mem(regval + constant, bw, value);
			break;

		case ADDRMODE::ABSOLUTE:
			constant = state.read_mem(state.peek(insnLength), bw);
			state.write_mem(constant, bw, value);
			break;

		default:
			ERR("invalid destination address mode");
			abort();
	}
}

std::string Operand::get_string() {
	std::string s{};
	s += "[Operand]:\n";
	s += "\tAddress Mode = " + std::to_string(numAddrmode) + "\n";
	s += "\tRegister = " + std::to_string(static_cast<int>(reg)) + "\n";
	s += "\tBw = " + std::to_string(bw) + "\n";
	return s;
}

void DoubleOp::execute(CPUState& state) {
	int a;
	int b;
	int value;
	switch (dop) {
		case DOUBLE_OP::MOV:
			value = src.get_value(state, length);
			dst.set_value(state, length, value);
			break;

		case DOUBLE_OP::ADD:
			a = src.get_value(state, length);
			b = dst.get_value(state, length);
			value = a + b;
			dst.set_value(state, length, value);
			state.set_arith_flags(a, b, value, bw);
			break;

		case DOUBLE_OP::SUBC:
			a = src.get_value(state, length);
			b = dst.get_value(state, length);
			value = b - a + state.get_flag_carry();
			dst.set_value(state, length, value);
			state.set_arith_flags(a, b, value, bw);
			break;

		case DOUBLE_OP::SUB:
			a = src.get_value(state, length);
			b = dst.get_value(state, length);
			value = b - a + 1;
			dst.set_value(state, length, value);
			state.set_arith_flags(a, b, value, bw);
			break;

		case DOUBLE_OP::CMP:
			a = src.get_value(state, length);
			b = dst.get_value(state, length);
			value = b - a;
			state.set_arith_flags(a, b, value, bw);
			break;

		default:
			ERR("unimplemented insn");
			std::cerr << get_string();
			abort();
	}
	return;
}

std::string DoubleOp::get_string() {
	std::string s{};
	s += "[Insn][Double Op] " + ENUMNAME(dop) + "\n";
	s += "\t" + src.get_string();
	s += "\t" + dst.get_string();
	return s;
}

void SingleOp::execute(CPUState& state) {
	ERR("unimplemented insn");
	std::cerr << get_string();
	abort();
	return;
}

std::string SingleOp::get_string() {
	std::string s{};
	s += "singleop placeholder\n";
	return s;
}

void Jump::execute(CPUState& state) {
	if (state.is_true(condition)) {
		state.set_pc(state.get_pc() + offset);
	}
	return;
}

std::string Jump::get_string() {
	std::string s{};
	s += "[Insn][Jump] " + ENUMNAME(condition) + "\n";
	s += "\tOffset = 0x" + int_to_hexstr(offset) + "\n";
	return s;
}
