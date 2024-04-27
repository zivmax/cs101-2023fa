#include "graph.hpp"

#include <cassert>

auto Graph::dijkstra(VertexID source) const -> std::vector<Weight> {
  struct HeapEntry {
    VertexID vertex;
    Weight dist;

    auto operator<=>(const HeapEntry &other) const {
      return dist <=> other.dist;
    }
  };

  std::priority_queue<HeapEntry, std::vector<HeapEntry>, std::greater<>> heap;
  std::vector<Weight> dist(numVertices(), infinity);

  dist[source] = 0;
  heap.push({.vertex = source, .dist = 0});

  while (!heap.empty()) {
    auto [current, currentDist] = heap.top();
    heap.pop();
    if (dist[current] != currentDist)
      continue;
    assert(dist[current] < infinity);
    for (auto [to, weight] : mAdjacent[current])
      if (dist[current] + weight < dist[to]) {
        dist[to] = dist[current] + weight;
        heap.push({.vertex = to, .dist = dist[to]});
      }
  }
  return dist;
}

auto Graph::bellmanFord(VertexID source) const
    -> std::optional<std::vector<Weight>> {
  std::vector<Weight> dist(numVertices(), infinity);
  dist[source] = 0;

  for (auto repeat = numVertices() - 1; repeat > 0; --repeat)
    for (VertexID from = 0; from != numVertices(); ++from)
      if (dist[from] < infinity)
        for (auto [to, weight] : mAdjacent[from])
          dist[to] = std::min(dist[to], dist[from] + weight);

  for (VertexID from = 0; from != numVertices(); ++from)
    if (dist[from] < infinity)
      for (auto [to, weight] : mAdjacent[from])
        if (dist[from] + weight < dist[to])
          return std::nullopt;

  return dist;
}