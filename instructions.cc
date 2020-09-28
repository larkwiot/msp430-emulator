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
		case ADDRMODE::INDIRECT_AUTOINCREMENT:
			regval = state.get_reg(reg);
			return state.read_mem(regval, bw);

		case ADDRMODE::IMMEDIATE:
			return state.read_mem(insnLength, bw);

		case ADDRMODE::CONSTANT:
			return state.get_reg(REG::CG);
	}

	if (addressMode == ADDRMODE::INDIRECT_AUTOINCREMENT) {
		state.registers.at(reg) += bw ? 1 : 2;
	}
}

void Operand::set_value(CPUState& state, int insnLength, uint16_t value) {
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
			abort();
	}
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
			dst.set_value(state, length, value & 0xFFFF);
			state.set_arith_flags(a, b, value, bw);
			break;

		case DOUBLE_OP::ADDC:
			break;

		case DOUBLE_OP::SUBC:
			break;

		case DOUBLE_OP::SUB:
			a = src.get_value(state, length);
			b = dst.get_value(state, length);
			value = b - a;
			dst.set_value(state, length, value & 0xFFFF);
			state.set_arith_flags(a, b, value, bw);
			break;

		case DOUBLE_OP::CMP:
			a = src.get_value(state, length);
			b = dst.get_value(state, length);
			value = b - a;
			state.set_arith_flags(a, b, value, bw);
			break;

		case DOUBLE_OP::DADD:
			break;

		case DOUBLE_OP::BIT:
			break;

		case DOUBLE_OP::BIC:
			break;

		case DOUBLE_OP::BIS:
			break;

		case DOUBLE_OP::XOR:
			break;

		case DOUBLE_OP::AND:
			break;
	}
	return;
}

void SingleOp::execute(CPUState& state) {
	return;
}

void Jump::execute(CPUState& state) {
	if (state.is_true(condition)) {
		state.set_pc(state.get_pc() + offset);
	}
	return;
}
