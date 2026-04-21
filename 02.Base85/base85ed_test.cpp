#include <gtest/gtest.h>
#include <random>
#include <vector>
#include <cstdint>
#include <string>

#include "base85ed.h"

// ---------------- utils ----------------

static std::vector<uint8_t> random_bytes(size_t n)
{
    std::vector<uint8_t> v(n);

    std::mt19937 gen(12345); // фиксируем seed → детерминированность тестов
    std::uniform_int_distribution<int> dist(0, 255);

    for (auto& x : v)
        x = static_cast<uint8_t>(dist(gen));

    return v;
}

// ---------------- tests ----------------

TEST(Base85, Empty)
{
    std::vector<uint8_t> data;

    auto enc = base85::encode(data);
    auto dec = base85::decode(enc);

    EXPECT_EQ(dec, data);
}

TEST(Base85, KnownString)
{
    std::string s = "hello world!";
    std::vector<uint8_t> data(s.begin(), s.end());

    auto dec = base85::decode(base85::encode(data));

    EXPECT_EQ(dec, data);
}

TEST(Base85, SingleByte)
{
    for (int i = 0; i < 256; ++i)
    {
        std::vector<uint8_t> data = {static_cast<uint8_t>(i)};

        auto dec = base85::decode(base85::encode(data));

        EXPECT_EQ(dec, data);
    }
}

TEST(Base85, SmallRandom)
{
    for (int n = 1; n <= 200; ++n)
    {
        auto data = random_bytes(n);

        auto dec = base85::decode(base85::encode(data));

        EXPECT_EQ(dec, data);
    }
}

TEST(Base85, AlignmentEdgeCases)
{
    for (int n = 1; n <= 100; ++n)
    {
        // специально ломаем границы блоков
        auto data = random_bytes(n * 4 - 1);

        auto dec = base85::decode(base85::encode(data));

        EXPECT_EQ(dec, data);
    }
}

TEST(Base85, LargeRandom)
{
    for (int n = 100; n <= 3000; n += 77)
    {
        auto data = random_bytes(n);

        auto dec = base85::decode(base85::encode(data));

        EXPECT_EQ(dec, data);
    }
}

TEST(Base85, DeterministicEncoding)
{
    auto data = random_bytes(128);

    auto e1 = base85::encode(data);
    auto e2 = base85::encode(data);

    EXPECT_EQ(e1, e2);
}
