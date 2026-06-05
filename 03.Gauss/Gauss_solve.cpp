#include "Gauss_solve.h"

#include <cmath>
#include <stdexcept>

GaussVector Gauss_solve(GaussMatrix &ab)
{
    const int row_count = ab.rows();
    const int column_count = ab.cols();

    if (column_count != row_count + 1)
    {
        throw std::invalid_argument("Expected an n x (n + 1) augmented matrix");
    }

    const double eps = 1e-12;

    for (int pivot_column = 0; pivot_column < row_count; ++pivot_column)
    {
        int pivot_row = pivot_column;
        double best_abs = std::fabs(ab(pivot_column, pivot_column));

        for (int row = pivot_column + 1; row < row_count; ++row)
        {
            const double current_abs = std::fabs(ab(row, pivot_column));
            if (current_abs > best_abs)
            {
                best_abs = current_abs;
                pivot_row = row;
            }
        }

        if (best_abs < eps)
        {
            throw std::runtime_error("The system has no unique solution");
        }

        if (pivot_row != pivot_column)
        {
            ab.row(pivot_column).swap(ab.row(pivot_row));
        }

        ab.row(pivot_column) /= ab(pivot_column, pivot_column);

        for (int row = 0; row < row_count; ++row)
        {
            if (row == pivot_column)
            {
                continue;
            }

            const double factor = ab(row, pivot_column);
            if (std::fabs(factor) > eps)
            {
                ab.row(row) -= factor * ab.row(pivot_column);
            }
        }
    }

    GaussVector result(row_count);
    for (int row = 0; row < row_count; ++row)
    {
        result(row) = ab(row, row_count);
    }

    return result;
}
