#include <algorithm>
#include <gtest/gtest.h>
#include <random>
#include <vector>

#include "sorting.h"

TEST(BubbleSortTest, HandlesBasicCases)
{
    const std::vector<std::vector<int>> tests = {
        {},
        {42},
        {1, 2, 3, 4, 5},
        {5, 4, 3, 2, 1},
        {3, -1, 3, 0, -5, 2, 2},
        {5, 1, 5, 1, 5, 1},
    };

    for (const auto& test : tests)
    {
        std::vector<int> actual = test;
        std::vector<int> expected = test;
        std::sort(expected.begin(), expected.end());

        bubble_sort(actual.begin(), actual.end());
        EXPECT_EQ(actual, expected);
    }
}

TEST(QuickSortTest, HandlesBasicCases)
{
    const std::vector<std::vector<int>> tests = {
        {},
        {7},
        {1, 2, 3, 4, 5, 6},
        {6, 5, 4, 3, 2, 1},
        {10, -2, 7, 7, 0, -2, 15, 1},
        {9, 8, 7, 6, 5, 4, 3},
    };

    for (const auto& test : tests)
    {
        std::vector<int> actual = test;
        std::vector<int> expected = test;
        std::sort(expected.begin(), expected.end());

        quick_sort(actual.begin(), actual.end());
        EXPECT_EQ(actual, expected);
    }
}

TEST(BothSortsTest, SameAsStdSortOnRandomData)
{
    std::mt19937 generator(20260605);
    std::uniform_int_distribution<int> distribution(-1000, 1000);

    for (int size = 0; size <= 300; size += 17)
    {
        std::vector<int> data(static_cast<std::size_t>(size));
        for (int& item : data)
        {
            item = distribution(generator);
        }

        std::vector<int> expected = data;
        std::sort(expected.begin(), expected.end());

        std::vector<int> bubble_data = data;
        bubble_sort(bubble_data.begin(), bubble_data.end());
        EXPECT_EQ(bubble_data, expected);

        std::vector<int> quick_data = data;
        quick_sort(quick_data.begin(), quick_data.end());
        EXPECT_EQ(quick_data, expected);
    }
}
