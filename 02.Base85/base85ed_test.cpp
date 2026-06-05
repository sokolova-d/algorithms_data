#include <gtest/gtest.h>

#include <cstdint>
#include <random>
#include <string>
#include <vector>

#include "base85ed.h"

namespace
{
std::vector<std::uint8_t> to_vector(const char* text)
{
    const std::string string_value(text);
    return std::vector<std::uint8_t>(string_value.begin(), string_value.end());
}

std::vector<std::uint8_t> random_bytes(std::size_t size)
{
    std::vector<std::uint8_t> data(size);
    std::mt19937 generator(12345);
    std::uniform_int_distribution<int> distribution(0, 255);

    for (std::uint8_t& byte : data)
    {
        byte = static_cast<std::uint8_t>(distribution(generator));
    }

    return data;
}
}

TEST(Base85Encode, KnownPythonCompatibleCases)
{
    const std::vector<std::pair<const char*, const char*>> cases = {
        {"", ""},
        {"F#", "1"},
        {"F){", "12"},
        {"F)}j", "123"},
        {"F)}kW", "1234"},
        {"Xk~0{Zy<MXa%^NF", "hello world!"},
        {"PjF>!K}i", "OpenAI"},
    };

    for (const auto& test_case : cases)
    {
        EXPECT_EQ(base85::encode(to_vector(test_case.second)), to_vector(test_case.first));
    }
}

TEST(Base85Decode, KnownPythonCompatibleCases)
{
    const std::vector<std::pair<const char*, const char*>> cases = {
        {"", ""},
        {"F#", "1"},
        {"F){", "12"},
        {"F)}j", "123"},
        {"F)}kW", "1234"},
        {"Xk~0{Zy<MXa%^NF", "hello world!"},
        {"PjF>!K}i", "OpenAI"},
    };

    for (const auto& test_case : cases)
    {
        EXPECT_EQ(base85::decode(to_vector(test_case.first)), to_vector(test_case.second));
    }
}

TEST(Base85RoundTrip, HandlesDifferentLengths)
{
    for (std::size_t size = 0; size <= 400; ++size)
    {
        const std::vector<std::uint8_t> data = random_bytes(size);
        EXPECT_EQ(base85::decode(base85::encode(data)), data);
    }
}

TEST(Base85Decode, RejectsInvalidCharacters)
{
    EXPECT_THROW(base85::decode(to_vector("abc\n")), std::runtime_error);
    EXPECT_THROW(base85::decode(to_vector("?")), std::runtime_error);
}
