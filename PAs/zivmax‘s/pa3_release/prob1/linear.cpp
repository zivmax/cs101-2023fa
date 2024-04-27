// @author: zivmax

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Edge
{
        int u, v, weight;
        Edge(int _u, int _v, int _weight) : u(_u), v(_v), weight(_weight) { }
        bool operator<(const Edge &e) const
        {
            return weight > e.weight; // Reverse sorting for maximum spanning tree
        }
};

struct DisjointSet
{
        vector<int> parent, rank;

        DisjointSet(int n) : parent(n + 1), rank(n + 1, 0)
        {
            for (int i = 0; i <= n; ++i)
            {
                parent[i] = i;
            }
        }

        int find(int u)
        {
            if (u != parent[u])
            {
                parent[u] = find(parent[u]);
            }
            return parent[u];
        }

        bool merge(int u, int v)
        {
            u = find(u);
            v = find(v);
            if (u == v) return false; // Cycle detected
            if (rank[u] > rank[v]) swap(u, v);
            parent[u] = v;
            if (rank[u] == rank[v]) ++rank[v];
            return true;
        }
};


long long maximumSpanningTree(int n, vector<Edge> &edges)
{
    sort(edges.begin(), edges.end());
    DisjointSet ds(n);
    long long maxValue = 0;

    for (const Edge &e : edges)
    {
        // As we're using 1-based indexing for vertices, we add 1 to the second vertex
        if (ds.merge(e.u, e.v))
        {
            maxValue += e.weight;
        }
    }

    return maxValue;
}

int main()
{
    int m, n;
    cin >> m >> n;
    vector<Edge> edges;

    for (int i = 0; i < m; ++i)
    {
        int l, r, c;
        cin >> l >> r >> c;
        edges.emplace_back(l, r + 1, c);
    }

    cout << maximumSpanningTree(n + 2, edges) << endl;

    return 0;
}
