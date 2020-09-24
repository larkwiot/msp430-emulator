#ifndef UTIL_H
#define UTIL_H

#include "includes.h"
#include "types.h"

std::vector<uchar> read_file(char* filename);

inline
constexpr uint16_t swap(uint16_t word) noexcept
{
    return ((word << 8) | (word >> 8));
}

template <int bits, typename I> inline
constexpr I sign_extend(I x) noexcept
{
    return ~(((x & static_cast<I>(pow(2, bits) - 1)) ^ bits) + 1);
}

template <typename E> inline
constexpr auto peel(E e) noexcept
{
    return static_cast<std::underlying_type_t<E>>(e);
}

ADDRMODE get_addrmode(int nmode, REG reg);

#endif // UTIL_H