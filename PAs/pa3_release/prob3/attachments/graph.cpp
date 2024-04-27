#include "graph.hpp"

auto Graph::dijkstra(VertexID source) const -> std::vector<Weight> {
  // FIXME: This is a naive O(V^2) implementation. Overwrite it to make it run
  // within O(ElogV) time, which is more efficient when E = o(V^2/logV).
  std::vector<Weight> dist(numVertices(), infinity);
  std::vector<bool> done(numVertices(), false);
  dist[source] = 0;
  for (auto rest = numVertices(); rest > 0; --rest) {
    VertexID current = -1;
    Weight minDist = infinity;
    for (VertexID i = 0; i != numVertices(); ++i)
      if (!done[i] && dist[i] < minDist) {
        current = i;
        minDist = dist[i];
      }
    done[current] = true;
    for (auto [to, weight] : mAdjacent[current])
      if (dist[current] + weight < dist[to])
        dist[to] = dist[current] + weight;
  }
  return dist;
}

auto Graph::bellmanFord(VertexID source) const
    -> std::optional<std::vector<Weight>> {
  // TODO: Implement this.
}