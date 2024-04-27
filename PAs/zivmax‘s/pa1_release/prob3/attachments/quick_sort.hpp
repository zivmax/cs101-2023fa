#include <iostream>
#include <vector>

auto random_choice(auto begin, auto end)
{
    // This is only a possible implementation of random_choice.
    // You can use this for your convenience or design a better one.
    std::size_t length = end - begin;
    std::size_t idx = std::rand() % length;
    return begin + idx;
}

void quick_sort(std::random_access_iterator auto begin,
                std::random_access_iterator auto end, auto compare)
{
    // TODO: IMPLEMENT THIS.
    if (end - begin <= 1) { return; }
    auto pivot = random_choice(begin, end);
    auto tail = end - 1;
    std::swap(*pivot, *tail);
    auto left = begin;
    auto right = end - 2;
    while (true)
    {
        while (compare(*left, *tail))
        {
            ++left;
        }
        while (compare(*tail, *right))
        {
            --right;
        }

        if (left < right)
        {
            std::swap(*left, *right);
        }
        else
        {
            break;
        }
    }

    std::swap(*left, *tail);
    quick_sort(begin, left, compare);
    quick_sort(left + 1, end, compare);
}

void quick_sort(std::random_access_iterator auto begin,
                std::random_access_iterator auto end)
{
    quick_sort(begin, end, std::less<>{});
}
