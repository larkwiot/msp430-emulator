#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "cpustate.h"
#include "includes.h"
#include "instructions.h"
#include "types.h"
#include "util.h"

struct CPU {
	CPUState state;
	uint16_t currInsn;
	std::unique_ptr<Insn> currDecInsn;

	CPU() { currDecInsn = nullptr; }
	~CPU() = default;

	void load_program(std::vector<uchar>& program);

	void fetch();
	void decode();
	void execute();
	void step();

	void run();

	std::string get_string();
};

#endif	// PROCESSOR_H