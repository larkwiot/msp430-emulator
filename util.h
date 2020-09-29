#ifndef UTIL_H
#define UTIL_H

#include "includes.h"
#include "types.h"

std::vector<uchar> read_file(char* filename);

constexpr uint16_t swap(uint16_t word) noexcept {
	return ((word << 8) | (word >> 8));
}

template <int bits, typename I>
constexpr I sign_extend(I x) noexcept {
	return ~(((x & static_cast<I>(pow(2, bits) - 1)) ^ bits) + 1);
}

ADDRMODE get_addrmode(int nmode, REG reg);

#define ERR(S) fprintf(stderr, "[!] error: " S)

template <typename I>
std::string int_to_hexstr(I x) {
	char hxstr[10];
	snprintf(hxstr, 10, "0x%x", x);
	return std::string(hxstr);
}

#endif	// UTIL_H