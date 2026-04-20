#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string>

#include "base85ed.h"

namespace base85
{

static constexpr uint32_t BASE = 85;
static constexpr uint32_t ASCII_OFFSET = 33;

static void encode_block(uint32_t value, std::vector<uint8_t>& out)
{
    char tmp[5];
    for (int i = 4; i >= 0; --i)
    {
        tmp[i] = static_cast<char>(value % BASE + ASCII_OFFSET);
        value /= BASE;
    }
    out.insert(out.end(), tmp, tmp + 5);
}

static uint32_t decode_block(const uint8_t* in)
{
    uint32_t value = 0;
    for (int i = 0; i < 5; ++i)
    {
        value = value * BASE + (in[i] - ASCII_OFFSET);
    }
    return value;
}

std::vector<uint8_t> encode(std::vector<uint8_t> const &bytes)
{
    std::vector<uint8_t> out;

    size_t i = 0;
    while (i < bytes.size())
    {
        uint32_t block = 0;
        int count = 0;

        for (; count < 4 && i < bytes.size(); ++count, ++i)
        {
            block |= (static_cast<uint32_t>(bytes[i]) << (24 - 8 * count));
        }

        if (count < 4)
        {
            block <<= (4 - count) * 8;
        }

        encode_block(block, out);
    }

    return out;
}

std::vector<uint8_t> decode(std::vector<uint8_t> const &b85str)
{
    if (b85str.size() % 5 != 0)
        throw std::runtime_error("Invalid Base85 length");

    std::vector<uint8_t> out;

    for (size_t i = 0; i < b85str.size(); i += 5)
    {
        uint32_t block = decode_block(&b85str[i]);

        uint8_t bytes[4];
        bytes[0] = (block >> 24) & 0xFF;
        bytes[1] = (block >> 16) & 0xFF;
        bytes[2] = (block >> 8) & 0xFF;
        bytes[3] = (block) & 0xFF;

        out.insert(out.end(), bytes, bytes + 4);
    }

    return out;
}

}