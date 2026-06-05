#include "util.h"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
std::string trim(const std::string &text)
{
    std::string::size_type first = 0;
    while (first < text.size() && std::isspace(static_cast<unsigned char>(text[first])) != 0)
    {
        ++first;
    }

    std::string::size_type last = text.size();
    while (last > first && std::isspace(static_cast<unsigned char>(text[last - 1])) != 0)
    {
        --last;
    }

    return text.substr(first, last - first);
}

std::vector<std::string> split_csv_line(const std::string &line)
{
    std::vector<std::string> cells;
    std::string cell;

    for (char ch : line)
    {
        if (ch == ',')
        {
            cells.push_back(trim(cell));
            cell.clear();
        }
        else
        {
            cell.push_back(ch);
        }
    }

    cells.push_back(trim(cell));
    return cells;
}

bool try_parse_double(const std::string &text, double &value)
{
    const std::string trimmed = trim(text);
    if (trimmed.empty())
    {
        return false;
    }

    char *end = nullptr;
    value = std::strtod(trimmed.c_str(), &end);

    if (end == trimmed.c_str())
    {
        return false;
    }

    while (*end != '\0')
    {
        if (std::isspace(static_cast<unsigned char>(*end)) == 0)
        {
            return false;
        }
        ++end;
    }

    return true;
}

bool parse_numeric_row(const std::vector<std::string> &cells, std::vector<double> &row)
{
    row.clear();
    row.reserve(cells.size());

    for (const std::string &cell : cells)
    {
        double value = 0.0;
        if (!try_parse_double(cell, value))
        {
            return false;
        }
        row.push_back(value);
    }

    return true;
}
} // namespace

GaussMatrix load_csv_to_matrix(const char *filename)
{
    std::ifstream in(filename);
    if (!in)
    {
        throw std::runtime_error("Cannot open input CSV file");
    }

    std::vector<std::vector<double>> rows;
    std::string line;
    bool first_line = true;
    std::size_t column_count = 0;

    while (std::getline(in, line))
    {
        if (trim(line).empty())
        {
            continue;
        }

        const std::vector<std::string> cells = split_csv_line(line);
        std::vector<double> row;

        if (!parse_numeric_row(cells, row))
        {
            if (first_line)
            {
                first_line = false;
                continue;
            }
            throw std::runtime_error("CSV contains a non-numeric row");
        }

        first_line = false;

        if (rows.empty())
        {
            column_count = row.size();
        }
        else if (row.size() != column_count)
        {
            throw std::runtime_error("CSV rows have different lengths");
        }

        rows.push_back(row);
    }

    if (rows.empty())
    {
        throw std::runtime_error("CSV does not contain numeric data");
    }

    if (column_count < 2)
    {
        throw std::runtime_error("Augmented matrix must contain at least two columns");
    }

    if (rows.size() + 1 != column_count)
    {
        throw std::runtime_error("CSV must contain an n x (n + 1) augmented matrix");
    }

    GaussMatrix matrix(static_cast<int>(rows.size()), static_cast<int>(column_count));
    for (int row_index = 0; row_index < matrix.rows(); ++row_index)
    {
        for (int column_index = 0; column_index < matrix.cols(); ++column_index)
        {
            matrix(row_index, column_index) = rows[static_cast<std::size_t>(row_index)][static_cast<std::size_t>(column_index)];
        }
    }

    return matrix;
}

void print_matrix_as_csv(std::ostream &out, const GaussMatrix &matrix, int prec)
{
    out << std::fixed << std::setprecision(prec);

    for (int row_index = 0; row_index < matrix.rows(); ++row_index)
    {
        for (int column_index = 0; column_index < matrix.cols(); ++column_index)
        {
            if (column_index > 0)
            {
                out << ',';
            }
            out << matrix(row_index, column_index);
        }
        out << '\n';
    }
}

void print_vector_as_csv(std::ostream &out, const GaussVector &vector, int prec)
{
    out << "x\n";
    out << std::fixed << std::setprecision(prec);

    for (int row_index = 0; row_index < vector.rows(); ++row_index)
    {
        out << vector(row_index) << '\n';
    }
}
