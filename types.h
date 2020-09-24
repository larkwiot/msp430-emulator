#ifndef TYPES_H
#define TYPES_H

#include "includes.h"

typedef unsigned char uchar;

enum class REG
{
    PC  = 0,
    SP  = 1,
    SR  = 2,
    CG  = 3,
    R4  = 4,
    R5  = 5,
    R6  = 6,
    R7  = 7,
    R8  = 8,
    R9  = 9,
    R10 = 10,
    R11 = 11,
    R12 = 12,
    R13 = 13,
    R14 = 14,
    R15 = 15,
    NONE,
    ANY,
};

enum class ADDRMODE
{
    //                      // what holds your data
    REGISTER,               // register
    INDEXED,                // *(register + constant)
    SYMBOLIC,               // *(pc       + constant)
    ABSOLUTE,               // *(constant)
    INDIRECT_REGISTER,      // *(register)
    INDIRECT_AUTOINCREMENT, // *(register); register++
    IMMEDIATE,              // constant
    CONSTANT,               // from CG
};

const static std::unordered_set<REG> specialRegs = {
    REG::PC,
    REG::SR,
    REG::CG,
};

const static std::map<std::pair<int, REG>, ADDRMODE> toAddrmode = {
    {{0, REG::ANY}, ADDRMODE::REGISTER},
    {{0, REG::PC},  ADDRMODE::REGISTER},
    {{0, REG::SR},  ADDRMODE::REGISTER},
    {{0, REG::CG},  ADDRMODE::REGISTER},
    
    {{1, REG::ANY}, ADDRMODE::INDEXED},
    {{1, REG::PC},  ADDRMODE::SYMBOLIC},
    {{1, REG::SR},  ADDRMODE::ABSOLUTE},
    {{1, REG::CG},  ADDRMODE::CONSTANT},
    
    {{2, REG::ANY}, ADDRMODE::INDIRECT_REGISTER},
    {{2, REG::PC},  ADDRMODE::INDIRECT_REGISTER},
    {{2, REG::SR},  ADDRMODE::CONSTANT},
    {{2, REG::CG},  ADDRMODE::CONSTANT},
    
    {{3, REG::ANY}, ADDRMODE::INDIRECT_AUTOINCREMENT},
    {{3, REG::PC},  ADDRMODE::IMMEDIATE},
    {{3, REG::SR},  ADDRMODE::CONSTANT},
    {{3, REG::CG},  ADDRMODE::CONSTANT},
};

const static std::unordered_set<ADDRMODE> bigModes = {
    ADDRMODE::ABSOLUTE,
    ADDRMODE::SYMBOLIC,
    ADDRMODE::INDEXED,
    ADDRMODE::IMMEDIATE,
};

enum class TYPE
{
    SINGLE = 0,
    JUMP   = 1,
    DOUBLE, // else
};

enum class DOUBLE_OP
{
    MOV     = 4,
    ADD     = 5,
    ADDC    = 6,
    SUBC    = 7,
    SUB     = 8,
    CMP     = 9,
    DADD    = 10,
    BIT     = 11,
    BIC     = 12,
    BIS     = 13,
    XOR     = 14,
    AND     = 15,
};

enum class SINGLE_OP
{
    RRC,
    SWPB,
    RRA,
    SXT,
    PUSH,
    CALL,
    RETI,
};

enum class COND
{
    NZ      = 0,
    EQ      = 1,
    NOCARRY = 2,
    CARRY   = 3,
    NEG     = 4,
    GE      = 5,
    LT      = 6,
    ABS     = 7,
};

// must match REG enum per specification
#define NUM_REGISTERS 16

#define ADDRESS_SPACE 0xFFFF

#endif // TYPES_H