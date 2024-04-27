#include <cassert>
#include <optional>

#include "graph.hpp"
#include "problem.hpp"

/// @brief Solve the given difference constraints problem.
/// @param problem
/// @return The solution, or @c std::nullopt if the problem has no solutions.
std::optional<Problem::Solution> solve(const Problem &problem)
{
    if (!problem.hasNegativeConstant())
    {
        // If all constants are non-negative, a trivial solution exists where
        // all variables are assigned the value 0.
        return Problem::Solution(problem.getNumVars(), 0);
    }

    // If there are negative constants, proceed with the graph approach.
    // Create a graph with an additional source vertex.
    Graph g(problem.getNumVars() + 1);

    // Connect the source vertex to all other vertices with zero-weight edges.
    for (Problem::VarID i = 0; i < problem.getNumVars(); ++i)
    {
        g.addEdge(problem.getNumVars(), i, 0);
    }

    // Add edges for all constraints.
    for (const auto &ct : problem.getConstraints())
    {
        g.addEdge(ct.var2, ct.var1, ct.constant);
    }

    auto dist = g.bellmanFord(problem.getNumVars());

    if (!dist)
    {
        return std::nullopt;
    }

    dist->pop_back(); // Remove the last element (the source vertex).
    return *dist;
}

int main()
{
    Problem p1(5, {
                      {0, 1, 0 },
                      {0, 4, -1},
                      {1, 4, 1 },
                      {2, 0, 5 },
                      {3, 0, 4 },
                      {3, 2, -1},
                      {4, 2, -3},
                      {4, 3, -3}
    });

    // This is a possible solution.
    assert(p1.isFeasible({-5, -3, 0, -1, -4}));

    auto sol = solve(p1);
    assert(sol);                        // The problem has a solution.
    assert(p1.isFeasible(sol.value())); // The solution is correct.

    return 0;
}