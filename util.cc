#include "util.h"

std::vector<uchar> read_file(char* filename)
{
    std::ifstream fh(filename, std::ios::binary);
    fh.unsetf(std::ios::skipws);
    
    std::streampos sz;

    fh.seekg(0, std::ios::end);
    sz = fh.tellg();
    fh.seekg(0, std::ios::beg);

    std::vector<uchar> result {};
    result.reserve(sz);

    result.insert(result.begin(), std::istream_iterator<uchar>(fh), std::istream_iterator<uchar>());

    return result;
}

ADDRMODE get_addrmode(int nmode, REG reg)
{
    if (!specialRegs.contains(reg))
    {
        reg = REG::ANY;
    }

    std::pair<int, REG> tup = {nmode, reg};

    auto x = toAddrmode.find(tup);
    if (x != toAddrmode.end())
    {
        return x->second;
    }
    
    abort();
}