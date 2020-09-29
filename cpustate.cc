#include "cpustate.h"

uint16_t CPUState::get_reg(REG reg) const {
	return registers.at(reg);
}

uint16_t CPUState::get_pc() const {
	return std::get<REG::PC>(registers);
}

void CPUState::set_reg(REG reg, uint16_t value) {
	registers.at(reg) = value;
	return;
}

void CPUState::set_pc(uint16_t value) {
	std::get<REG::PC>(registers) = value;
	return;
}

uint16_t CPUState::get_flags() const {
	return std::get<REG::SR>(registers);
}

bool CPUState::get_flag_carry() const {
	return (get_flags() & 0x1) == 1;
}

constexpr void CPUState::set_flag_carry() {
	std::get<REG::SR>(registers) |= 0x1;
	return;
}

constexpr void CPUState::clear_flag_carry() {
	std::get<REG::SR>(registers) &= 0xFFFE;
	return;
}

bool CPUState::get_flag_zero() const {
	return (get_flags() & 0x2) == 0x2;
}

constexpr void CPUState::set_flag_zero() {
	std::get<REG::SR>(registers) |= 0x2;
	return;
}

constexpr void CPUState::clear_flag_zero() {
	std::get<REG::SR>(registers) &= 0xFFFD;
	return;
}

bool CPUState::get_flag_negative() const {
	return (get_flags() & 0x4) == 0x4;
}

constexpr void CPUState::set_flag_negative() {
	std::get<REG::SR>(registers) |= 0x4;
	return;
}

constexpr void CPUState::clear_flag_negative() {
	std::get<REG::SR>(registers) &= 0xFFFB;
	return;
}

bool CPUState::get_flag_overflow() const {
	return (get_flags() & 0x100) == 0x100;
}

constexpr void CPUState::set_flag_overflow() {
	std::get<REG::SR>(registers) |= 0x100;
	return;
}

constexpr void CPUState::clear_flag_overflow() {
	std::get<REG::SR>(registers) &= 0xFEFF;
	return;
}

bool CPUState::is_true(COND cond) const noexcept {
	switch (cond) {
		case COND::NZ:
			return !get_flag_zero();

		case COND::EQ:
			return get_flag_zero();

		case COND::NOCARRY:
			return !get_flag_carry();

		case COND::CARRY:
			return get_flag_carry();

		case COND::NEG:
			return get_flag_negative();

		case COND::GE:
			return get_flag_negative() == get_flag_overflow();

		case COND::LT:
			return get_flag_negative() != get_flag_overflow();

		case COND::ABS:
			return true;
	}
}

uint16_t CPUState::read_mem(size_t address, bool bw) const {
	if (bw) {
		return memory.at(address) & 0xFF;
	}
	return swap(memory.at(address));
}

void CPUState::write_mem(size_t address, bool bw, uint16_t value) {
	if (bw) {
		memory.at(address) = static_cast<uchar>(value & 0xFF);
	} else {
		memory.at(address) = swap(value);
	}
	return;
}

uint16_t CPUState::peek(int offset) const {
	return read_mem(get_pc() + offset, false);
}

void CPUState::set_arith_flags(uint16_t a, uint16_t b, int result, bool bw) {
	int negflag = bw ? 0x80 : 0x8000;
	int carflag = bw ? 0x100 : 0x10000;
	result == 0 ? set_flag_zero() : clear_flag_zero();

	(result & negflag) != 0 ? set_flag_negative() : clear_flag_negative();
	(result & carflag) != 0 ? set_flag_carry() : clear_flag_carry();

	if (((result & negflag) != (a & negflag)) &&
			((a & negflag) == (b & negflag))) {
		set_flag_overflow();
	} else {
		clear_flag_overflow();
	}

	return;
}

std::string CPUState::get_string() {
	std::string s{};
	s += "[CPU State] registers:\n";
	int ri = 0;
	for (auto r : registers) {
		s += "\tR" + std::to_string(ri) + " " + std::to_string(r) + "\n";
	}
	return s;
}
