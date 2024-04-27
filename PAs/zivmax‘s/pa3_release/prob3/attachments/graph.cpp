#include "graph.hpp"
#include <queue> // Add this include statement

auto Graph::dijkstra(VertexID source) const -> std::vector<Weight>
{
    std::vector<Weight> dist(numVertices(), infinity);
    std::vector<bool> done(numVertices(), false);
    dist[source] = 0;

    // Use a priority queue to select the vertex with the minimum distance
    std::priority_queue<std::pair<Weight, VertexID>, std::vector<std::pair<Weight, VertexID>>, std::greater<>> pq;
    pq.push({0, source});

    while (!pq.empty())
    {
        VertexID current = pq.top().second;
        pq.pop();

        if (done[current]) { continue; }

        done[current] = true;

        for (auto [to, weight] : mAdjacent[current])
        {
            if (dist[current] + weight < dist[to])
            {
                dist[to] = dist[current] + weight;
                pq.push({dist[to], to});
            }
        }
    }

    return dist;
}


auto Graph::bellmanFord(VertexID source) const
    -> std::optional<std::vector<Weight>>
{
    std::size_t V = numVertices();
    std::vector<Weight> dist(V, infinity);
    dist[source] = 0;

    bool containsNegativeEdge = false;

    // Relax all edges V - 1 times.
    for (std::size_t i = 1; i < V; ++i)
    {
        for (std::size_t u = 0; u < V; ++u)
        {
            for (const auto &edge : mAdjacent[u])
            {
                VertexID v = edge.to;
                Weight weight = edge.weight;
                if (weight < 0) { containsNegativeEdge = true; }

                // Relaxation step
                if (dist[u] != infinity && dist[v] > dist[u] + weight)
                {
                    dist[v] = dist[u] + weight;
                }
            }
        }
    }

    if (containsNegativeEdge)
    { // Check for negative-weight cycles.
        for (std::size_t u = 0; u < V; ++u)
        {
            for (const auto &edge : mAdjacent[u])
            {
                VertexID v = edge.to;
                Weight weight = edge.weight;
                if (dist[u] != infinity && dist[v] > dist[u] + weight)
                {
                    // Negative cycle detected
                    return std::nullopt;
                }
            }
        }
    }

    return dist;
}