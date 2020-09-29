#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpustate.h"
#include "includes.h"
#include "types.h"
#include "util.h"

struct Operand {
	ADDRMODE addressMode;
	int numAddrmode;
	REG reg;
	bool bw;

	Operand() = default;

	Operand(REG rg, int nmode, bool bw) : numAddrmode(nmode), reg(rg), bw(bw) {
		addressMode = get_addrmode(numAddrmode, reg);
	}

	uint8_t get_value(CPUState& state, int insnLength);
	void set_value(CPUState& state, int insnLength, uint16_t value);
	std::string get_string();
};

struct Insn {
	TYPE type;
	int length;

	Insn() = default;
	virtual ~Insn() = default;

	virtual void execute(CPUState& state) = 0;
	virtual std::string get_string() = 0;
};

struct DoubleOp : Insn {
	DOUBLE_OP dop;
	Operand src;
	bool bw;
	Operand dst;

	DoubleOp(uint16_t op) noexcept {
		type = TYPE::DOUBLE;
		length = 16;
		bw = ((op >> 6) & 1);
		dop = static_cast<DOUBLE_OP>((op >> 12) & 0xF);

		src = Operand{static_cast<REG>((op >> 8) & 0xF), ((op >> 4) & 2), bw};

		dst = Operand{static_cast<REG>(op & 0xF), ((op >> 7) & 1), bw};

		if (bigModes.contains(src.addressMode) ||
				bigModes.contains(dst.addressMode)) {
			length += 2;
		}

		return;
	}

	~DoubleOp() = default;

	void execute(CPUState& state) override;
	std::string get_string() override;
};

struct SingleOp : Insn {
	SINGLE_OP sop;
	bool bw;
	Operand dst;

	SingleOp(uint16_t op) noexcept {
		type = TYPE::SINGLE;
		length = 16;
		bw = ((op >> 6) & 1);
		sop = static_cast<SINGLE_OP>((op >> 7) & 0x7);

		dst = Operand{static_cast<REG>(op & 0xF), ((op >> 4) & 2), bw};

		if (bigModes.contains(dst.addressMode)) {
			length += 2;
		}
		return;
	}

	~SingleOp() = default;

	void execute(CPUState& state) override;
	std::string get_string() override;
};

struct Jump : Insn {
	COND condition;
	int16_t offset;

	Jump(uint16_t op) noexcept {
		type = TYPE::JUMP;
		length = 16;
		condition = static_cast<COND>((op >> 10) & 0x7);

		offset = sign_extend<10>(op);
		return;
	}

	~Jump() = default;

	void execute(CPUState& state) override;
	std::string get_string() override;
};

#endif	// INSTRUCTIONS_H