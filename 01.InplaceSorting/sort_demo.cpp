#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include "collvalue.h"
#include "sorting.h"

using CollInt = CollectingValue<int>;

std::vector<CollInt> generate_data(std::size_t n)
{
    std::vector<CollInt> data(n);
    std::iota(data.begin(), data.end(), CollInt(0));
    return data;
}

void shuffle_data(std::vector<CollInt>& data)
{
    static std::mt19937 generator(20260605);
    std::shuffle(data.begin(), data.end(), generator);
}

void print_stats(std::size_t n, const char* algorithm)
{
    std::cout << n << '\t' << algorithm << "\t\t" << CollInt::comps << "\t\t"
              << CollInt::swaps << "\t\t" << CollInt::moves << '\n';
}

int main()
{
    const std::vector<std::size_t> sizes = {100, 500, 1000, 2000};

    std::cout << "N\tAlgo\t\tComps\t\tSwaps\t\tMoves\n";
    std::cout << "----------------------------------------------------------------\n";

    for (std::size_t n : sizes)
    {
        auto number_data = generate_data(n);

        shuffle_data(number_data);
        CollInt::reset_stats();
        std::sort(number_data.begin(), number_data.end());
        print_stats(n, "std::sort");

        shuffle_data(number_data);
        CollInt::reset_stats();
        bubble_sort(number_data.begin(), number_data.end());
        print_stats(n, "Bubble");

        shuffle_data(number_data);
        CollInt::reset_stats();
        quick_sort(number_data.begin(), number_data.end());
        print_stats(n, "Quick");

        std::cout << "--------------------------------------------------------------\n";
    }

    return 0;
}
