#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

#include "sorting.h"

TEST(BubbleSortTest, EmptyVector) {
    std::vector<int> data;
    bubble_sort(data.begin(), data.end());
    EXPECT_TRUE(data.empty());
}

TEST(BubbleSortTest, OneElement) {
    std::vector<int> data = {42};
    bubble_sort(data.begin(), data.end());
    EXPECT_EQ(data, std::vector<int>({42}));
}

TEST(BubbleSortTest, AlreadySorted) {
    std::vector<int> data = {1, 2, 3, 4, 5};
    bubble_sort(data.begin(), data.end());
    EXPECT_EQ(data, std::vector<int>({1, 2, 3, 4, 5}));
}

TEST(BubbleSortTest, ReverseOrder) {
    std::vector<int> data = {5, 4, 3, 2, 1};
    bubble_sort(data.begin(), data.end());
    EXPECT_EQ(data, std::vector<int>({1, 2, 3, 4, 5}));
}

TEST(BubbleSortTest, DuplicatesAndNegatives) {
    std::vector<int> data = {3, -1, 3, 0, -5, 2, 2};
    bubble_sort(data.begin(), data.end());
    EXPECT_EQ(data, std::vector<int>({-5, -1, 0, 2, 2, 3, 3}));
}

TEST(QuickSortTest, EmptyVector) {
    std::vector<int> data;
    quick_sort(data.begin(), data.end());
    EXPECT_TRUE(data.empty());
}

TEST(QuickSortTest, OneElement) {
    std::vector<int> data = {7};
    quick_sort(data.begin(), data.end());
    EXPECT_EQ(data, std::vector<int>({7}));
}

TEST(QuickSortTest, AlreadySorted) {
    std::vector<int> data = {1, 2, 3, 4, 5, 6};
    quick_sort(data.begin(), data.end());
    EXPECT_EQ(data, std::vector<int>({1, 2, 3, 4, 5, 6}));
}

TEST(QuickSortTest, ReverseOrder) {
    std::vector<int> data = {6, 5, 4, 3, 2, 1};
    quick_sort(data.begin(), data.end());
    EXPECT_EQ(data, std::vector<int>({1, 2, 3, 4, 5, 6}));
}

TEST(QuickSortTest, DuplicatesAndNegatives) {
    std::vector<int> data = {10, -2, 7, 7, 0, -2, 15, 1};
    quick_sort(data.begin(), data.end());
    EXPECT_EQ(data, std::vector<int>({-2, -2, 0, 1, 7, 7, 10, 15}));
}

TEST(BothSortsTest, SameAsStdSortSmallCases) {
    std::vector<std::vector<int>> tests = {
        {},
        {1},
        {2, 1},
        {3, 1, 2},
        {4, 1, 3, 2},
        {9, 8, 7, 6, 5, 4, 3},
        {5, 1, 5, 1, 5, 1},
        {-10, 100, 0, -5, 8, 8, 3}
    };

    for (const auto& test : tests) {
        std::vector<int> expected = test;
        std::sort(expected.begin(), expected.end());

        std::vector<int> bubble_data = test;
        bubble_sort(bubble_data.begin(), bubble_data.end());
        EXPECT_EQ(bubble_data, expected);

        std::vector<int> quick_data = test;
        quick_sort(quick_data.begin(), quick_data.end());
        EXPECT_EQ(quick_data, expected);
    }
}

TEST(BothSortsTest, SameAsStdSortBiggerCase) {
    std::vector<int> data = {
        15, 3, 9, 8, 5, 2, 7, 1, 6, 4,
        12, 11, 14, 13, 10, 0, -1, 20, 18, 17,
        19, 16, -5, 22, 21, 3, 3, 8, 8, -2
    };

    std::vector<int> expected = data;
    std::sort(expected.begin(), expected.end());

    std::vector<int> bubble_data = data;
    bubble_sort(bubble_data.begin(), bubble_data.end());
    EXPECT_EQ(bubble_data, expected);

    std::vector<int> quick_data = data;
    quick_sort(quick_data.begin(), quick_data.end());
    EXPECT_EQ(quick_data, expected);
}
