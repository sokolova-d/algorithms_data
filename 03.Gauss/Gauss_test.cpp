#include <gtest/gtest.h>

#include "Gauss_solve.h"

#include <cmath>
#include <random>
#include <sstream>
#include <stdexcept>

namespace
{
GaussMatrix make_augmented_matrix(const GaussMatrix &a, const GaussVector &x)
{
    GaussMatrix ab(a.rows(), a.cols() + 1);

    for (int row = 0; row < a.rows(); ++row)
    {
        double right_part = 0.0;
        for (int column = 0; column < a.cols(); ++column)
        {
            ab(row, column) = a(row, column);
            right_part += a(row, column) * x(column);
        }
        ab(row, a.cols()) = right_part;
    }

    return ab;
}
}

TEST(GaussSolve, SmallSolve)
{
    GaussMatrix ab(2, 3);
    ab(0, 0) = 1.1;
    ab(0, 1) = 2.2;
    ab(0, 2) = 3.3;
    ab(1, 0) = 4.4;
    ab(1, 1) = 5.5;
    ab(1, 2) = 6.6;

    const GaussVector result = Gauss_solve(ab);

    EXPECT_NEAR(result(0), -1.0, 1e-9);
    EXPECT_NEAR(result(1), 2.0, 1e-9);
}

TEST(GaussSolve, PivotingIsUsed)
{
    GaussMatrix ab(2, 3);
    ab(0, 0) = 0.0;
    ab(0, 1) = 2.0;
    ab(0, 2) = 4.0;
    ab(1, 0) = 1.0;
    ab(1, 1) = 3.0;
    ab(1, 2) = 7.0;

    const GaussVector result = Gauss_solve(ab);

    EXPECT_NEAR(result(0), 1.0, 1e-9);
    EXPECT_NEAR(result(1), 2.0, 1e-9);
}

TEST(GaussSolve, SingularMatrixThrows)
{
    GaussMatrix ab(2, 3);
    ab(0, 0) = 1.0;
    ab(0, 1) = 2.0;
    ab(0, 2) = 3.0;
    ab(1, 0) = 2.0;
    ab(1, 1) = 4.0;
    ab(1, 2) = 6.0;

    EXPECT_THROW(Gauss_solve(ab), std::runtime_error);
}

TEST(GaussSolve, ReproducibleRandomLargeSystem)
{
    const int size = 60;
    std::mt19937 generator(20260528);
    std::uniform_real_distribution<double> distribution(-5.0, 5.0);

    GaussMatrix a(size, size);
    GaussVector expected(size);

    for (int row = 0; row < size; ++row)
    {
        expected(row) = distribution(generator);
        double row_sum = 0.0;
        for (int column = 0; column < size; ++column)
        {
            const double value = distribution(generator);
            a(row, column) = value;
            row_sum += std::fabs(value);
        }
        a(row, row) += row_sum + 1.0;
    }

    GaussMatrix ab = make_augmented_matrix(a, expected);
    const GaussVector actual = Gauss_solve(ab);

    for (int row = 0; row < size; ++row)
    {
        EXPECT_NEAR(actual(row), expected(row), 1e-7);
    }
}

TEST(CsvUtil, PrintsVectorAsCsv)
{
    GaussVector vector(2);
    vector(0) = -1.0;
    vector(1) = 2.0;

    std::ostringstream out;
    print_vector_as_csv(out, vector, 3);

    EXPECT_EQ(out.str(), "x\n-1.000\n2.000\n");
}
