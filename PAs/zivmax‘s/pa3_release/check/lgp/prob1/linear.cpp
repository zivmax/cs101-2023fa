#include <iostream>
#include <vector>
#include <algorithm>

class Disjoint_Set_Union
{
private:
    std::vector<int> father;
    std::vector<int> rank;
public:
    Disjoint_Set_Union(int n) : father(n + 1), rank(n + 1, 0)
    {
        for (int i = 1; i <= n; i++)
            father[i] = i;
    }

    int find(int x)
    {
        return father[x] == x ? x : father[x] = find(father[x]);
    }

    void merge(int x, int y)
    {
        int fx = find(x), fy = find(y);
        if (fx == fy)
            return;
        if (rank[fx] < rank[fy])
            father[fx] = fy;
        else
        {
            father[fy] = fx;
            if (rank[fx] == rank[fy])
                rank[fx]++;
        }
    }
};


struct vec{
    int l, r;
    size_t value;
};



int main() {
    std::vector<vec> v;
    int m, n;
    std::cin >> m >> n;
    vec tmp;

    for(int i = 0; i < m; i++) {
        std::cin >> tmp.l >> tmp.r >> tmp.value;
        tmp.r++;
        v.push_back(tmp);
    }

    std::sort(v.begin(), v.end(), [](vec a, vec b) {
        return a.value > b.value;
    });

    size_t ret = 0;

    Disjoint_Set_Union dsu(n + 1);

    for (const auto &edge : v) {
        if (dsu.find(edge.l) != dsu.find(edge.r)) {
            ret += edge.value;
            dsu.merge(edge.l, edge.r);
        }
    }
    std::cout << ret;
    return 0;
}