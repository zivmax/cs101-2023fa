#ifndef HUFFMAN_CALCULATOR_HPP
#define HUFFMAN_CALCULATOR_HPP

#include <algorithm>
#include <memory>
#include <tuple>
#include <vector>

using std::make_shared;
using std::pair;
using std::shared_ptr;
using std::vector;

struct Node
{
        size_t occur;
        size_t depth;
};

using Root = pair<size_t, shared_ptr<vector<Node>>>;
using NodeSum = pair<size_t, size_t>;
using RootsIter = vector<Root>::iterator;

size_t get_huffman_length(const vector<NodeSum> &data)
{

    vector<Root> roots;
    vector<NodeSum> nodes = data;

    // Sort the input ascending by occurrence
    sort(nodes.begin(), nodes.end());

    while (!(roots.size() == 1 && nodes.empty()))
    {
        // Pick the 2 smallest nodes/roots
        NodeSum smallest_2[2];
        bool picked_root[2] = {false}; // indicates how many roots were picked
        vector<Root> picked_roots;
        for (int i = 0; i < 2; ++i)
        {
            if (roots.empty() || (!nodes.empty() && nodes.front().first < roots.front().first))
            {
                smallest_2[i] = nodes.front();
                --nodes.front().second;
                if (nodes.front().second == 0)
                {
                    nodes.erase(nodes.begin());
                }
            }
            else
            {
                smallest_2[i] = {roots.front().first, 0};
                picked_roots.push_back(roots.front());
                roots.erase(roots.begin());
                picked_root[i] = true;
            }
        }

        // Combine the nodes/roots into a new root
        size_t newOccur = 0;
        shared_ptr<vector<Node>> newRoot = make_shared<vector<Node>>();
        for (size_t i = 0; i < 2; ++i)
        {
            newOccur += smallest_2[i].first;
            if (picked_root[i])
            {
                for (const Node &node : *picked_roots.front().second)
                {
                    newRoot->push_back({node.occur, node.depth + 1});
                }
                picked_roots.erase(picked_roots.begin());
            }
            else
            {
                newRoot->push_back({smallest_2[i].first, 1});
            }
        }

        // Insert the new root back into the roots vector and nodes vector
        RootsIter iter_roots = std::lower_bound(roots.begin(), roots.end(), newOccur, [](const Root &a, const size_t &b) { return a.first < b; });
        roots.insert(iter_roots, {newOccur, std::move(newRoot)});
    }

    // Calculate the length of the Huffman encoded sequence
    size_t length = 0;
    for (const Root &root : roots)
    {
        for (const Node &node : *root.second)
        {
            length += node.occur * node.depth;
        }
    }

    return length;
}

#endif // HUFFMAN_CALCULATOR_HPP
