#ifndef UTIL_H
#define UTIL_H

#include "includes.h"
#include "types.h"

#define ERR(S) fprintf(stderr, "[!] error: " S "\n")
#define ENUMNAME(E) std::string(magic_enum::enum_name(E))

std::vector<uchar> read_file(char* filename);

template <int bits, typename I>
constexpr I sign_extend(I x) noexcept {
	return ~(((x & static_cast<I>(pow(2, bits) - 1)) ^ bits) + 1);
}

ADDRMODE get_addrmode(int nmode, REG reg);

template <typename I>
std::string int_to_hexstr(I x) {
	char hxstr[10];
	snprintf(hxstr, 10, "0x%x", x);
	return std::string(hxstr);
}

#endif	// UTIL_H