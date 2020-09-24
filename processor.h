#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "includes.h"
#include "types.h"
#include "util.h"
#include "instructions.h"
#include "state.h"

struct CPU
{
    CPUState state;
    uint16_t currInsn;
    std::unique_ptr<Insn> currDecInsn;

    CPU() = default;
    ~CPU() = default;

    void load_program(std::vector<uchar>& program);

    void fetch();
    void decode();
    void execute();
    void step();
};

#endif // PROCESSOR_H