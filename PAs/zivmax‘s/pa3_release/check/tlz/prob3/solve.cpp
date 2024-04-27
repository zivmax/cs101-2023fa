#include <cassert>
#include <optional>

#include "graph.hpp"
#include "problem.hpp"

/// @brief Solve the given difference constraints problem.
/// @param problem
/// @return The solution, or @c std::nullopt if the problem has no solutions.
std::optional<Problem::Solution> solve(const Problem &problem)
{
    // Create a graph with an additional source vertex.
    Graph g(problem.getNumVars() + 1);

    // Connect the source vertex to all other vertices.
    for (Problem::VarID i = 0; i < problem.getNumVars(); ++i)
    {
        g.addEdge(problem.getNumVars(), i, 0);
    }

    // Add edges for all constraints.
    for (const auto &ct : problem.getConstraints())
    {
        g.addEdge(ct.var2, ct.var1, ct.constant);
    }

    // Compute shortest paths.
    std::optional<std::vector<Graph::Weight>> dist;
    if (problem.hasNegativeConstant())
    {
        dist = g.bellmanFord(problem.getNumVars());
    }
    else
    {
        dist = g.dijkstra(problem.getNumVars());
    }

    // If a negative cycle exists, the problem has no solution.
    if (!dist) { return std::nullopt; }

    // The shortest path distances to all vertices are the solutions.
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