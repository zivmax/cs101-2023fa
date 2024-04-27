#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>

struct Edge {
  int from, to, weight;
};

class DisjointSets {
  mutable std::vector<int> mParent;
  std::vector<int> mSize;

public:
  explicit DisjointSets(std::size_t n) : mParent(n), mSize(n, 1) {
    std::iota(begin(mParent), end(mParent), 0);
  }
  int findParent(int x) const {
    if (x == mParent[x])
      return x;
    else
      return (mParent[x] = findParent(mParent[x]));
  }
  bool merge(int x, int y) {
    x = findParent(x);
    y = findParent(y);
    if (x == y)
      return false;
    if (mSize[x] < mSize[y]) {
      mParent[x] = y;
      mSize[y] += mSize[x];
    } else {
      mParent[y] = x;
      mSize[x] += mSize[y];
    }
    return true;
  }
};

int main() {
  int m, n;
  std::cin >> m >> n;
  std::vector<Edge> edges;
  edges.reserve(m);
  while (m--) {
    int l, r, c;
    std::cin >> l >> r >> c;
    edges.push_back({l - 1, r, c});
  }
  std::ranges::sort(edges, std::ranges::greater{}, &Edge::weight);
  DisjointSets ds(n + 1);
  long long ans = 0;
  for (auto [from, to, weight] : edges) {
    if (ds.merge(from, to))
      ans += weight;
  }
  std::cout << ans << std::endl;
  return 0;
}