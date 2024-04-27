#ifndef HUFFMAN_CALCULATOR_HPP
#define HUFFMAN_CALCULATOR_HPP

#include <algorithm>
#include <stdexcept>
#include <tuple>
#include <vector>

size_t get_huffman_length(const std::vector<std::pair<size_t, size_t>> &data)
{
    // Check ascending sort
    if (std::is_sorted(data.begin(), data.end(), std::less_equal<std::pair<size_t, size_t>>()))
    {
        throw std::runtime_error("Ascending");
    }

    // Check descending sort
    if (std::is_sorted(data.begin(), data.end(), std::greater_equal<std::pair<size_t, size_t>>()))
    {
        throw std::runtime_error("Descending");
    }

    // Return wrong output
    return 0;
}
#endif // HUFFMAN_CALCULATOR_HPP