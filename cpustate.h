#ifndef STATE_H
#define STATE_H

#include "includes.h"
#include "types.h"
#include "util.h"

struct CPUState {
	std::array<uint16_t, NUM_REGISTERS> registers;
	std::array<uint8_t, ADDRESS_SPACE + 1> memory;

	uint16_t get_reg(REG reg) const;
	uint16_t get_pc() const;
	void set_reg(REG reg, uint16_t value);
	void set_pc(uint16_t value);

	inline uint16_t get_flags() const;

	void set_flags(uint16_t resultValue);

	inline bool get_flag_carry() const;
	inline constexpr void set_flag_carry();
	constexpr void clear_flag_carry();

	inline bool get_flag_zero() const;
	inline constexpr void set_flag_zero();
	constexpr void clear_flag_zero();

	inline bool get_flag_negative() const;
	inline constexpr void set_flag_negative();
	constexpr void clear_flag_negative();

	inline bool get_flag_overflow() const;
	inline constexpr void set_flag_overflow();
	constexpr void clear_flag_overflow();

	bool is_true(COND cond) const noexcept;

	uint16_t read_mem(size_t address, bool bw) const;
	void write_mem(size_t address, bool bw, uint16_t value);

	uint16_t peek(int offset) const;

	void set_arith_flags(uint16_t a, uint16_t b, int result, bool bw);
};

#endif	// STATE_H