#include "util.h"

std::vector<uchar> read_file(char* filename) {
	std::ifstream fh(filename, std::ios::binary);

	if (fh.fail()) {
		ERR("could not open file");
		abort();
	}

	fh.unsetf(std::ios::skipws);

	std::streampos sz;
	fh.seekg(0, std::ios::end);
	sz = fh.tellg();
	fh.seekg(0, std::ios::beg);

	printf("[*] running %s, size %d bytes\n", filename, static_cast<int>(sz));

	std::vector<uchar> result{};
	result.reserve(sz);

	result.insert(result.begin(), std::istream_iterator<uchar>(fh),
								std::istream_iterator<uchar>());

	return result;
}

ADDRMODE get_addrmode(int nmode, REG reg) {
	if (nmode == 0) {
		return ADDRMODE::REGISTER;
	} else if (reg == REG::CG) {
		return ADDRMODE::CONSTANT;
	} else if (reg != REG::PC && reg != REG::SR) {
		reg = REG::ANY;
	}

	std::pair<int, REG> tup = {nmode, reg};

	auto x = toAddrmode.find(tup);
	if (x != toAddrmode.end()) {
		return x->second;
	}

	ERR("could not resolve address mode from int and reg");
	std::cerr << "[num and reg] " << std::to_string(nmode) << " " << ENUMNAME(reg)
						<< "\n";
	abort();
}

std::string int_to_bytes_str(uint16_t x) {
	char hxstr[6];
	int low = x & 0xFF;
	int high = (x >> 8) & 0xFF;
	snprintf(hxstr, 6, "%x %x", low, high);
	return std::string(hxstr);
}