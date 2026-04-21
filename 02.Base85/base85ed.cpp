#include <vector>
#include <cstdint>
#include <stdexcept>
#include <algorithm>

#include "base85ed.h"

namespace base85
{

static constexpr uint32_t BASE = 85;
static constexpr uint32_t OFFSET = 33;

// ---------------- encode ----------------
std::vector<uint8_t> encode(const std::vector<uint8_t>& bytes)
{
    std::vector<uint8_t> out;
    size_t i = 0;

    while (i < bytes.size())
    {
        uint32_t block = 0;
        size_t chunk_size = std::min<size_t>(4, bytes.size() - i);

        // Читаем до 4 байт
        for (int j = 0; j < 4; ++j)
        {
            block <<= 8;
            if (i < bytes.size())
                block |= bytes[i++];
        }

        // Кодируем в 5 символов Base85
        char tmp[5];
        for (int j = 4; j >= 0; --j)
        {
            tmp[j] = static_cast<char>(block % BASE + OFFSET);
            block /= BASE;
        }

        // Согласно стандарту (как в Python/RFC 1924), если блок был неполным,
        // нужно выводить только chunk_size + 1 символов.
        out.insert(out.end(), tmp, tmp + (chunk_size + 1));
    }

    return out;
}

// ---------------- decode ----------------
std::vector<uint8_t> decode(const std::vector<uint8_t>& b85str)
{
    std::vector<uint8_t> out;
    size_t i = 0;

    while (i < b85str.size())
    {
        uint32_t block = 0;
        size_t chunk_size = std::min<size_t>(5, b85str.size() - i);
        
        if (chunk_size < 2) break; 

        for (size_t j = 0; j < 5; ++j) // Исправлено: int -> size_t
        {
            block *= BASE;
            if (j < chunk_size) {
                block += (b85str[i++] - OFFSET);
            } else {
                block += 84; 
            }
        }

        // Исправлено: int -> size_t и безопасное приведение типа
        for (size_t j = 0; j < chunk_size - 1; ++j)
        {
            out.push_back((block >> (24 - j * 8)) & 0xFF);
        }
    }

    return out;
}

} // namespace base85