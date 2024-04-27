// @author: yuguo

#include <iostream>
#include <vector>
#include<algorithm>

class Vect{
public:
    size_t l, r, w;
    Vect(size_t left, size_t right, size_t weight):l(left), r(right), w(weight){}
};
class DisjointSet{
private:
    std::vector<std::size_t> parents;
    std::vector<std::size_t> rank;
public:
    DisjointSet(std::size_t n){
        for(std::size_t i = 0;i < n;i++){
            parents.push_back(i);
            rank.push_back(1);
        }
    }
    std::size_t find(std::size_t a){
        // the element is not unioned
        if(parents[a] == a){
            return a;
        }
        parents[a] = find(parents[a]);

        return parents[a];
    }
    void Union(std::size_t a, std::size_t b){
        std::size_t a_parent = find(a);
        std::size_t b_parent = find(b);
        // already unioned
        if(a_parent == b_parent){
            return;
        }

        if(rank[a_parent] > rank[b_parent]){
            rank[a_parent] += rank[b_parent];
            parents[b_parent] = parents[a_parent];

        }
        else{
            rank[b_parent] += rank[a_parent];
            parents[a_parent] = parents[b_parent];
        }
    }
    bool isUnion(std::size_t a, std::size_t b){
        return find(a) == find(b);
    }
};
bool compare(Vect a, Vect b){
    return a.w > b.w;
}
int main(){
    std::size_t m, n;
    std::cin >> m >> n;
    std::vector<Vect> storage;
    std::size_t left, right, weight;
    std::size_t res = 0;
    for(std::size_t i = 0;i < m;++i){
        std::cin >> left >> right >> weight;
        storage.push_back(Vect(left, right, weight));
    }

    std::sort(storage.begin(), storage.end(),compare);

    // starting from 1 to n+1
    DisjointSet d_set = DisjointSet(n + 1);
    for(std::size_t i = 0;i < m;++i){
        auto l = storage[i].l;
        auto r = storage[i].r;
        if(!d_set.isUnion(l-1, r)){
            d_set.Union(l-1, r);
            res += storage[i].w;
        }
    }
    std::cout << res << std::endl;
    return 0;
}