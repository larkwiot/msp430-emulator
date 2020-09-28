#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "includes.h"
#include "types.h"
#include "util.h"
#include "cpustate.h"

struct Operand
{
    ADDRMODE addressMode;
    int numAddrmode;
    REG reg;
    bool bw;

    Operand() = default;

    Operand(REG rg, int nmode, bool bw)
        : numAddrmode(nmode), reg(rg), bw(bw)
    {
        addressMode = get_addrmode(numAddrmode, reg);
    }

    uint8_t get_value(CPUState& state, int insnLength);
    void set_value(CPUState& state, int insnLength, uint16_t value);
};

struct Insn
{
    TYPE type;
    int length;

    Insn() = default;
    virtual ~Insn() = default;

    virtual void execute(CPUState& state) = 0;
};

struct DoubleOp : Insn
{
    DOUBLE_OP dop;
    Operand src;
    bool bw;
    Operand dst;

    DoubleOp(uint16_t op, DOUBLE_OP dop) noexcept
        : dop(dop)
    {
        type = TYPE::DOUBLE;
        length = 16;
        bw = ((op >> 6) & 1);

        src = Operand {
            static_cast<REG>((op >> 8) & 0xF),
            ((op >> 4) & 2),
            bw
        };

        dst = Operand {
            static_cast<REG>(op & 0xF),
            ((op >> 7) & 1),
            bw
        };

        if (bigModes.contains(src.addressMode)
            || bigModes.contains(dst.addressMode))
        {
            length += 2;
        }
        
        return;
    }

    ~DoubleOp() = default;

    void execute(CPUState& state) override;
};

struct SingleOp : Insn
{
    SINGLE_OP sop;
    bool bw;
    Operand dst;

    SingleOp(uint16_t op, SINGLE_OP sop) noexcept
        : sop(sop)
    {
        type = TYPE::SINGLE;
        length = 16;
        bw = ((op >> 6) & 1);

        dst = Operand {
            static_cast<REG>(op & 0xF),
            ((op >> 4) & 2),
            bw
        };

        if (bigModes.contains(dst.addressMode))
        {
            length += 2;
        }
        return;
    }

    ~SingleOp() = default;

    void execute(CPUState& state) override;
};

struct Jump : Insn
{
    COND condition;
    int16_t offset;

    Jump(uint16_t op, COND cond) noexcept
        : condition(cond)
    {
        type = TYPE::JUMP;
        length = 16;

        offset = sign_extend<10>(op);
        return;
    }

    ~Jump() = default;

    void execute(CPUState& state) override;
};

#endif // INSTRUCTIONS_H