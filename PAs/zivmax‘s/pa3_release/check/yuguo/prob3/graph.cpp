#include "graph.hpp"


auto Graph::dijkstra(VertexID source) const -> std::vector<Weight> {
  // FIXME: This is a naive O(V^2) implementation. Overwrite it to make it run
  // within O(ElogV) time, which is more efficient when E = o(V^2/logV).
  std::vector<Weight> dist(numVertices(), infinity);
  std::vector<bool> done(numVertices(), false);
  dist[source] = 0;
  auto cmp = [](const AdjListEntry x, const AdjListEntry y) {
    return x.weight > y.weight;
  };
  std::priority_queue<AdjListEntry, std::vector<AdjListEntry>, decltype(cmp)> MinHeap(cmp);
  MinHeap.push({source, 0});// 0 is weight
  while(!MinHeap.empty()){
    auto this_v = MinHeap.top();
    MinHeap.pop();
    // has already been visited
    if(done[this_v.to])
      continue;
    done[this_v.to] = true;
    for(auto& [to, weight]: mAdjacent[this_v.to]){
    if(dist[this_v.to] + weight < dist[to]){
        dist[to] = dist[this_v.to] + weight;
        MinHeap.push({to, dist[to]});
    }
    }
  }
  return dist;
  // for test
  // std::vector<Weight> dist(numVertices(), infinity);
  // std::vector<bool> done(numVertices(), false);
  // dist[source] = 0;
  // for (auto rest = numVertices(); rest > 0; --rest) {
  //   VertexID current = -1;
  //   Weight minDist = infinity;
  //   for (VertexID i = 0; i != numVertices(); ++i)
  //     if (!done[i] && dist[i] < minDist) {
  //       current = i;
  //       minDist = dist[i];
  //     }
  //   done[current] = true;
  //   for (auto [to, weight] : mAdjacent[current])
  //     if (dist[current] + weight < dist[to])
  //       dist[to] = dist[current] + weight;
  // }
  // return dist;
}

auto Graph::bellmanFord(VertexID source) const
    -> std::optional<std::vector<Weight>> {
  // TODO: Implement this.
  std::vector<Weight> dist(numVertices(), infinity);
  dist[source] = 0;
  // every iteration
  for(VertexID i = 1; i < numVertices();++i){
    // go through all the edges
    for(VertexID index = 0; index < numVertices(); ++ index){
      for(auto [to, weight]: mAdjacent[index]){
        if(dist[index] + weight < dist[to] && dist[index] != infinity){
            dist[to] = dist[index] + weight;
        }
      }
    }
  }
  // check for negative cycles
  for(VertexID index = 0; index < numVertices(); ++ index){
      for(auto [to, weight]: mAdjacent[index]){
        auto& current_dist = dist[to];
        auto loose_dist = dist[index] + weight;
        if(loose_dist < current_dist && dist[index] != infinity){
          return std::nullopt;
        }
      }
    }
  
  return dist;

}