#include <gtest/gtest.h>
#include <random>
#include <vector>
#include <cstdint>

#include "base85ed.h"

static std::vector<uint8_t> str2vec(const std::string& s)
{
    return std::vector<uint8_t>(s.begin(), s.end());
}

static std::vector<uint8_t> random_bytes(size_t n)
{
    std::vector<uint8_t> v(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 255);

    for (auto &x : v) x = static_cast<uint8_t>(dist(gen));
    return v;
}

TEST(Base85, Empty)
{
    std::vector<uint8_t> empty;
    EXPECT_EQ(base85::decode(base85::encode(empty)), empty);
}

TEST(Base85, KnownStrings)
{
    std::string s = "hello world!";
    auto enc = base85::encode(str2vec(s));
    auto dec = base85::decode(enc);

    EXPECT_EQ(dec, str2vec(s));
}

TEST(Base85, RoundTripSmall)
{
    for (int i = 1; i < 100; ++i)
    {
        auto data = random_bytes(i);
        auto enc = base85::encode(data);
        auto dec = base85::decode(enc);

        EXPECT_EQ(dec, data);
    }
}

TEST(Base85, RoundTripLarge)
{
    for (int i = 100; i < 5000; i += 123)
    {
        auto data = random_bytes(i);
        auto enc = base85::encode(data);
        auto dec = base85::decode(enc);

        EXPECT_EQ(dec, data);
    }
}

TEST(Base85, SingleByte)
{
    auto data = std::vector<uint8_t>{0x00};
    auto enc = base85::encode(data);
    auto dec = base85::decode(enc);

    EXPECT_EQ(dec[0], 0x00);
}

TEST(Base85, MaxBytes)
{
    auto data = std::vector<uint8_t>{255, 254, 253, 252};
    auto enc = base85::encode(data);
    auto dec = base85::decode(enc);

    EXPECT_EQ(dec, data);
}

TEST(Base85, Deterministic)
{
    auto data = random_bytes(128);

    auto e1 = base85::encode(data);
    auto e2 = base85::encode(data);

    EXPECT_EQ(e1, e2);
}