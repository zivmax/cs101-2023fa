#include "../huffman_calculator.hpp"
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

using input_type = std::vector<std::pair<std::size_t, std::size_t>>;
using return_type = std::size_t;
std::map<int, std::pair<input_type, return_type>> testcases = {
    {1, {{{7, 1}, {3, 2}, {2, 1}, {1, 2}}, 39}                   },
    {2, {{{9, 1}, {3, 2}, {1, 2}, {2, 1}}, 41}                   },
    {3, {{{1, 4}}, 8}                                            },
    {4, {{{2, 2}}, 4}                                            },
    {5, {{{1, 6}, {2, 3}, {5, 4}, {6, 2}, {9, 1}, {100, 1}}, 347}},
    {6, {{{1, 142857}}, 2452139}                                 },
};

int main()
{
    for (const auto &item : testcases)
    {
        std::cout << "Local testcase: " << item.first
                  << "  Your: " << get_huffman_length(item.second.first)
                  << "\tAnswer: " << item.second.second << std::endl;
    }
    return 0;
}
