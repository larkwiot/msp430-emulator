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
			std::cerr << "[address mode] " << ENUMNAME(addressMode) << "\n";
			abort();
	}
}

std::string Operand::get_string() {
	std::string s{};
	s += "[Operand]:\n";
	s += "\tAddress Mode = " + ENUMNAME(addressMode) + "\n";
	s += "\tRegister = " + ENUMNAME(reg) + "\n";
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

std::string DoubleOp::get_name() {
	return ENUMNAME(dop);
}

void SingleOp::execute(CPUState& state) {
	int value;
	switch (sop) {
		case SINGLE_OP::RRA:
			value = dst.get_value(state, length);
			(value & 0x1) ? state.set_flag_carry() : state.clear_flag_carry();
			value >>= 1;
			dst.set_value(state, length, value);
			break;

		case SINGLE_OP::SXT:
			value = dst.get_value(state, length);
			value &= 0xFF;
			value = sign_extend<16>(value);
			dst.set_value(state, length, value);
			break;

		case SINGLE_OP::PUSH:
			state.set_reg(REG::SP, state.get_reg(REG::SP) - 2);
			value = dst.get_value(state, length);
			state.write_mem(state.get_reg(REG::SP), bw, value);
			break;

		case SINGLE_OP::CALL:
			state.set_reg(REG::SP, state.get_reg(REG::SP) - 2);
			value = state.get_pc() + length;
			state.write_mem(state.get_reg(REG::SP), bw, value);
			break;

		case SINGLE_OP::RETI:
			value = state.read_mem(state.get_reg(REG::SP), bw);
			state.set_reg(REG::SR, value);
			state.set_reg(REG::SP, state.get_reg(REG::SP) + 2);
			value = state.read_mem(state.get_reg(REG::SP), bw);
			state.set_pc(value);
			state.set_reg(REG::SP, state.get_reg(REG::SP) + 2);
			break;

		default:
			ERR("unimplemented insn");
			std::cout << "[insn] " << get_name() << "\n";
			abort();
	}
	return;
}

std::string SingleOp::get_string() {
	std::string s{};
	s += "singleop placeholder\n";
	return s;
}

std::string SingleOp::get_name() {
	return ENUMNAME(sop);
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

std::string Jump::get_name() {
	return ENUMNAME(condition);
}
