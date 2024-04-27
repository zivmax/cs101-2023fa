#include <iostream>
#include <vector>

int main() {
  int n, m;
  std::cin >> n >> m;

  std::vector<int> v(n + 1);
  std::vector<int> c(n + 1);

  for (auto i = 1; i <= n; ++i)
    std::cin >> v[i] >> c[i];
  
  std::vector dpForward(n + 2, std::vector(m + 1, 0LL));
  for (auto i = 1; i <= n; ++i) {
    for (auto j = 0; j < c[i] && j <= m; ++j)
      dpForward[i][j] = dpForward[i - 1][j];
    for (auto j = c[i]; j <= m; ++j)
      dpForward[i][j] = std::max(dpForward[i - 1][j], dpForward[i - 1][j - c[i]] + v[i]);
  }

  std::vector dpBackward(n + 2, std::vector(m + 1, 0LL));
  for (auto i = n; i >= 1; --i) {
    for (auto j = 0; j < c[i] && j <= m; ++j)
      dpBackward[i][j] = dpBackward[i + 1][j];
    for (auto j = c[i]; j <= m; ++j)
      dpBackward[i][j] = std::max(dpBackward[i + 1][j], dpBackward[i + 1][j - c[i]] + v[i]);
  }

  auto msv = dpForward[n][m];
  std::vector<int> am;
  for (auto i = 1, j = m; i <= n; ++i)
    if (c[i] <= j && dpBackward[i][j] == dpBackward[i + 1][j - c[i]] + v[i]) {
      j -= c[i];
      am.push_back(i);
    }
  for (auto i : am)
    std::cout << i << ' ';
  std::cout << '\n';
  for (auto i : am) {
    auto msvi = 0LL;
    for (auto j = 0; j <= m; ++j)
      msvi = std::max(msvi, dpForward[i - 1][j] + dpBackward[i + 1][m - j]);
    std::cout << v[i] + msvi - msv << ' ';
  }
  std::cout << std::endl;
  return 0;
}