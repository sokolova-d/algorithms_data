#include "Gauss_solve.h"
#include "util.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

int main(int argc, const char *argv[])
{
    if (argc != 2 && argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " input.csv [output.csv]\n";
        return 1;
    }

    try
    {
        GaussMatrix ab = load_csv_to_matrix(argv[1]);
        const GaussVector result = Gauss_solve(ab);

        if (argc == 3)
        {
            std::ofstream out(argv[2]);
            if (!out)
            {
                throw std::runtime_error("Cannot open output CSV file");
            }
            print_vector_as_csv(out, result);
        }
        else
        {
            print_vector_as_csv(std::cout, result);
        }
    }
    catch (const std::exception &error)
    {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
