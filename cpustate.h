#ifndef STATE_H
#define STATE_H

#include "includes.h"
#include "types.h"
#include "util.h"

struct CPUState {
	std::array<uint16_t, NUM_REGISTERS> registers;
	std::array<uint8_t, ADDRESS_SPACE + 1> memory;

	CPUState() {
		set_pc(0xC000);
		return;
	}

	~CPUState() = default;

	uint16_t get_reg(REG reg) const;
	uint16_t get_pc() const;
	void set_reg(REG reg, uint16_t value);
	void set_pc(uint16_t value);

	uint16_t get_flags() const;

	void set_flags(uint16_t resultValue);

	bool get_flag_carry() const;
	constexpr void set_flag_carry();
	constexpr void clear_flag_carry();

	bool get_flag_zero() const;
	constexpr void set_flag_zero();
	constexpr void clear_flag_zero();

	bool get_flag_negative() const;
	constexpr void set_flag_negative();
	constexpr void clear_flag_negative();

	bool get_flag_overflow() const;
	constexpr void set_flag_overflow();
	constexpr void clear_flag_overflow();

	bool is_true(COND cond) const noexcept;

	uint16_t read_mem(size_t address, bool bw) const;
	void write_mem(size_t address, bool bw, uint16_t value);

	uint16_t peek(int offset) const;

	void set_arith_flags(uint16_t a, uint16_t b, int result, bool bw);

	std::string get_string();
};

#endif	// STATE_H