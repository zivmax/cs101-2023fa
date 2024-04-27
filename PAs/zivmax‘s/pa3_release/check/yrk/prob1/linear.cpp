// @author: yrk

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Vector
{
    long long int l, r, c;
};

class UnionFind
{
public:
    vector<long long int> parent;
    UnionFind(long long int size) : parent(size)
    {
        for (long long int i = 0; i < size; ++i)
        {
            parent[i] = i;
        }
    }
    long long find(long long x)
    {
        return x == parent[x] ? x : (parent[x] = find(parent[x]));
    }
    bool unionSet(long long int x, long long int y)
    {
        long long int px = find(x);
        long long int py = find(y);
        if (px == py)
        {
            return false;
        }
        parent[px] = py;
        return true;
    }
};

int main()
{
    long long int m, n;
    cin >> m >> n;
    vector<Vector> vectors(m);
    for (long long int i = 0; i < m; i++)
    {
        cin >> vectors[i].l >> vectors[i].r >> vectors[i].c;
    }
    sort(vectors.begin(), vectors.end(), [](const Vector &a, const Vector &b)
         { return a.c > b.c; });

    UnionFind uf(10000000);
    long long int count = 0;
    for (const auto &vec : vectors)
    {
        if (uf.unionSet(vec.l, vec.r + 1))
        {
            count += vec.c;
        }
    }
    cout << count << endl;
    return 0;
}
