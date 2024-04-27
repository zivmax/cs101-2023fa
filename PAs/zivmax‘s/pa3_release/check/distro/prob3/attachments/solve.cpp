#include <optional>
#include <cassert>

#include "problem.hpp"
#include "graph.hpp"

/// @brief Solve the given difference constraints problem.
/// @param problem 
/// @return The solution, or @c std::nullopt if the problem has no solutions.
std::optional<Problem::Solution> solve(const Problem &problem) {
  // TODO: Your code here.
}

int main() {
  Problem p1(5, {{0, 1, 0},
                 {0, 4, -1},
                 {1, 4, 1},
                 {2, 0, 5},
                 {3, 0, 4},
                 {3, 2, -1},
                 {4, 2, -3},
                 {4, 3, -3}});

  // This is a possible solution.
  assert(p1.isFeasible({-5, -3, 0, -1, -4}));

  auto sol = solve(p1);
  assert(sol);                        // The problem has a solution.
  assert(p1.isFeasible(sol.value())); // The solution is correct.

  return 0;
}