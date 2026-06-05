#ifndef BASE85ED_H
#define BASE85ED_H

#include <cstdint>
#include <vector>

namespace base85
{

std::vector<std::uint8_t> encode(const std::vector<std::uint8_t>& bytes);
std::vector<std::uint8_t> decode(const std::vector<std::uint8_t>& b85str);

} // namespace base85

#endif // BASE85ED_H
