#include <iostream>
#include <vector>

auto random_choice(auto begin, auto end) {
  // This is only a possible implementation of random_choice.
  // You can use this for your convenience or design a better one.
  std::size_t length = end - begin;
  std::size_t idx = std::rand() % length;
  return begin + idx;
}

void quick_sort(std::random_access_iterator auto begin,
                std::random_access_iterator auto end, auto compare) {
  // TODO: IMPLEMENT THIS.
}

void quick_sort(std::random_access_iterator auto begin,
                std::random_access_iterator auto end) {
  quick_sort(begin, end, std::less<>{});
}
