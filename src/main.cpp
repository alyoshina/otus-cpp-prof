#include "matrix.h"

#include <iostream>
#include <exception>
#include <cassert>
#include <iterator>
#include <algorithm>
#include <ranges>

using namespace matrix;

/**
 * @brief Using infinite matrix filled with values, matrix depth is 2,
 * default value is 0, type of matrix objects is int.
 */
int main()
{   
    try {
        //create Matrix with empty value 0
        Matrix<int, 0> matrix;
 
        //fill from [0,0] to [9,9]) values from 0 to 9,
        //from [0,9] to [9,0]) values from 9 to 0
        int N = 10;
        for (int i = 0; i < N; i++) {
            matrix[i][i] = i;
            matrix[i][N - 1 - i] = N - 1 - i;
        }

        //output matrix fragment from [1,1] to [8,8]. Space between columns.
        //Each line matrices on a new console line
        {
            static_assert(std::input_or_output_iterator<Matrix<int, 0>::Iterator>);
            static_assert(std::ranges::input_range<Matrix<int, 0>>);

            auto rng = matrix | std::views::filter([from = 0u, to = 9u] (auto t) {
                return std::get<0>(t) > from && std::get<0>(t) < to
                        && std::get<1>(t) > from && std::get<1>(t) < to;
                });
            if (!rng.empty()) {
                std::size_t l_x;
                {
                    std::size_t y;
                    int v;
                    std::tie(l_x, y, v) = *rng.begin();
                    std::cout << v;
                }
                std::ranges::for_each(std::ranges::next(rng.begin()), rng.end(), [&l_x] (auto t) {
                    std::size_t x, y;
                    int v;
                    std::tie(x, y, v) = t;
                    if (l_x == x) {
                        std::cout << " " << v;
                    } else {
                        std::cout << std::endl << v;
                        l_x = x;
                    }
                });
                std::cout << std::endl;
            }
        }
        
        //number of occupied cells
        std::cout << matrix.size() << std::endl;

        //all occupied cells along with their positions
        for (auto c: matrix) {
            std::size_t x;
            std::size_t y;
            int v;
            std::tie(x, y, v) = c;
            std::cout << x << y << v << std::endl;
        }

        ((matrix[100][100] = 314) = 0) = 217;
        
    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
