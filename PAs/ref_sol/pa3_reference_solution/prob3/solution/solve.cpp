#include <optional>
#include <cassert>

#include "problem.hpp"
#include "graph.hpp"

/// @brief Solve the given difference constraints problem.
/// @param problem
/// @return The solution, or @c std::nullopt if the problem has no solutions.
std::optional<Problem::Solution> solve(const Problem &problem) {
  if (problem.hasNegativeConstant()) {
    Graph graph(problem.getNumVars() + 1);
    for (auto [v1, v2, c] : problem.getConstraints())
      graph.addEdge(v2, v1, c);
    auto source = problem.getNumVars();
    for (Problem::VarID v = 0; v != problem.getNumVars(); ++v)
      graph.addEdge(source, v, 0);
    return graph.bellmanFord(source);
  } else
    return Problem::Solution(problem.getNumVars(), 0);
}
