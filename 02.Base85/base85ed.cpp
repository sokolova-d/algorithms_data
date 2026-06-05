#include "base85ed.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace base85
{
namespace
{
constexpr std::uint32_t BASE = 85;
constexpr char ALPHABET[] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "!#$%&()*+-;<=>?@^_`{|}~";

std::array<int, 256> make_decode_table()
{
    std::array<int, 256> table{};
    table.fill(-1);

    for (std::size_t index = 0; index < BASE; ++index)
    {
        table[static_cast<unsigned char>(ALPHABET[index])] = static_cast<int>(index);
    }

    return table;
}

const std::array<int, 256> DECODE_TABLE = make_decode_table();

void append_encoded_block(std::vector<std::uint8_t>& out, std::uint32_t block,
                          std::size_t symbols_count)
{
    std::array<std::uint8_t, 5> encoded{};

    for (std::size_t index = encoded.size(); index > 0; --index)
    {
        encoded[index - 1] = static_cast<std::uint8_t>(ALPHABET[block % BASE]);
        block /= BASE;
    }

    out.insert(out.end(), encoded.begin(), encoded.begin() + symbols_count);
}
} // namespace

std::vector<std::uint8_t> encode(const std::vector<std::uint8_t>& bytes)
{
    std::vector<std::uint8_t> out;
    out.reserve((bytes.size() + 3) / 4 * 5);

    for (std::size_t position = 0; position < bytes.size(); position += 4)
    {
        const std::size_t chunk_size = std::min<std::size_t>(4, bytes.size() - position);
        std::uint32_t block = 0;

        for (std::size_t index = 0; index < 4; ++index)
        {
            block <<= 8;
            if (index < chunk_size)
            {
                block |= bytes[position + index];
            }
        }

        append_encoded_block(out, block, chunk_size + 1);
    }

    return out;
}

std::vector<std::uint8_t> decode(const std::vector<std::uint8_t>& b85str)
{
    std::vector<std::uint8_t> out;
    out.reserve(b85str.size() / 5 * 4 + 4);

    for (std::size_t position = 0; position < b85str.size(); position += 5)
    {
        const std::size_t chunk_size = std::min<std::size_t>(5, b85str.size() - position);
        if (chunk_size == 1)
        {
            throw std::runtime_error("Invalid Base85 block length");
        }

        std::uint64_t block = 0;
        for (std::size_t index = 0; index < 5; ++index)
        {
            int digit = BASE - 1;
            if (index < chunk_size)
            {
                const unsigned char symbol = b85str[position + index];
                digit = DECODE_TABLE[symbol];
                if (digit < 0)
                {
                    throw std::runtime_error("Invalid Base85 character");
                }
            }

            block = block * BASE + static_cast<std::uint32_t>(digit);
        }

        if (block > 0xFFFFFFFFULL)
        {
            throw std::runtime_error("Invalid Base85 value");
        }

        const std::size_t bytes_count = chunk_size - 1;
        for (std::size_t index = 0; index < bytes_count; ++index)
        {
            const std::size_t shift = 24 - index * 8;
            out.push_back(static_cast<std::uint8_t>((block >> shift) & 0xFF));
        }
    }

    return out;
}

} // namespace base85
