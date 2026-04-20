#ifndef SORTING_H
#define SORTING_H

#include "collvalue.h"

template <typename Iterator>
void bubble_sort(Iterator begin, Iterator end) {
    if (begin == end) {
        return;
    }

    for (Iterator last = end; last != begin; --last) {
        bool changed = false;

        for (Iterator it = begin; it + 1 != last; ++it) {
            if (*(it + 1) < *it) {
                swap(*it, *(it + 1));
                changed = true;
            }
        }

        if (!changed) {
            return;
        }
    }
}

template <typename Iterator>
void quick_sort(Iterator begin, Iterator end) {
    if (end - begin <= 1) {
        return;
    }

    Iterator left = begin;
    Iterator right = end - 1;
    auto pivot = *(begin + (end - begin) / 2);

    while (left <= right) {
        while (*left < pivot) {
            ++left;
        }
        while (pivot < *right) {
            --right;
        }

        if (left <= right) {
            swap(*left, *right);
            ++left;
            --right;
        }
    }

    if (begin < right + 1) {
        quick_sort(begin, right + 1);
    }
    if (left < end) {
        quick_sort(left, end);
    }
}

#endif
