#ifndef SORTING_H
#define SORTING_H

#include "collvalue.h"

#include <algorithm>

template <typename Iterator>
void bubble_sort(Iterator begin, Iterator end)
{
    if (begin == end)
    {
        return;
    }

    for (Iterator last = end; last != begin; --last)
    {
        bool changed = false;

        for (Iterator it = begin; it + 1 != last; ++it)
        {
            if (*(it + 1) < *it)
            {
                using std::swap;
                swap(*it, *(it + 1));
                changed = true;
            }
        }

        if (!changed)
        {
            return;
        }
    }
}

template <typename Iterator>
void quick_sort(Iterator begin, Iterator end)
{
    if (end - begin <= 1)
    {
        return;
    }

    auto pivot = *(begin + (end - begin) / 2);
    Iterator less = begin;
    Iterator current = begin;
    Iterator greater = end;

    while (current < greater)
    {
        if (*current < pivot)
        {
            using std::swap;
            swap(*less, *current);
            ++less;
            ++current;
        }
        else if (pivot < *current)
        {
            --greater;
            using std::swap;
            swap(*current, *greater);
        }
        else
        {
            ++current;
        }
    }

    quick_sort(begin, less);
    quick_sort(greater, end);
}

#endif // SORTING_H
