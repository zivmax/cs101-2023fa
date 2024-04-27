#ifndef HUFFMAN_CALCULATOR_HPP
#define HUFFMAN_CALCULATOR_HPP

#include <algorithm>
#include <memory>
#include <tuple>
#include <vector>

using std::make_shared;
using std::pair;
using std::shared_ptr;
using std::sort;
using std::tuple;
using std::vector;


// Multi Abstraction Data Type, for store different types of abstraction data in one container
struct MultiAbs
{
        size_t first;
        size_t second;
        bool is_node_sum = false; // {occurrence, amount of nodes having that same occurrence}
        bool is_tree_sum = false;   // {occurrence summation, amount of perfect trees having that same occurrence summation}
        bool is_tree = false;    // {occurrence summation of every descendants of the root, amount of descendants of the root}

        MultiAbs() { }

        // Define a constructor that takes an initializer list
        MultiAbs(std::initializer_list<size_t> init)
        {
            if (init.size() >= 2)
            {
                auto it = init.begin();
                first = *it;
                second = *(++it);
                is_node_sum = (init.size() >= 3) ? *(++it) != 0 : false;
                is_tree_sum = (init.size() >= 4) ? *(++it) != 0 : false;
                is_tree = (init.size() >= 5) ? *(++it) != 0 : false;
            }
        }

        MultiAbs(const std::pair<size_t, size_t> &p) : first(p.first), second(p.second), is_node_sum(true) { }

        // Define a less-than operator for MultiAbs
        bool operator<(const MultiAbs &other) const
        {
            return first < other.first;
        }
};

using Root = MultiAbs;
using NodeSum = MultiAbs;
using RootsIter = vector<Root>::iterator;
using NodesIter = vector<NodeSum>::iterator;

size_t get_huffman_length(const std::vector<std::pair<size_t, size_t>> &data)
{
    size_t length = 0;

    vector<NodeSum> nodes;

    // Store data in nodes first
    for (auto &i : data)
    {
        nodes.push_back(i);
    }

    // Sort the input ascending by occurrence
    sort(nodes.begin(), nodes.end());

    do
    {
        // Pick the 2 smallest element by `first`
        MultiAbs smallest_2[2];
        bool over_pick = false;

        for (int i = 0; i < 2; ++i)
        {
            if (!nodes.empty())
            {
                smallest_2[i] = nodes.front();
                nodes.erase(nodes.begin());
            }
            else
            {
                smallest_2[i] = {0, 0, false, false, true};
                over_pick = true;
            }
        }

        size_t newOccur = 0;
        size_t newNodesAmount = 0;

        // When the smallest is a trees or a node summary, we can handle it quickly.
        if (smallest_2[0].second != 1 && !smallest_2[0].is_tree)
        {
            // Push back the uselessly picked nodes/roots
            if (!over_pick) { nodes.insert(nodes.begin(), smallest_2[1]); }

            // When the smallest is a node with corresponding node summary of more than 1, we can handle it quickly.
            newOccur = smallest_2[0].first * 2;
            size_t total_nodes = smallest_2[0].second / 2;

            // Insert the new trees back into the vector, which maintains the ascending order
            NodesIter iter_nodes = std::lower_bound(nodes.begin(), nodes.end(), newOccur, [](const NodeSum &a, const size_t &b) { return a.first < b; });
            nodes.insert(iter_nodes, {newOccur, total_nodes, false, true, false});
            if (smallest_2[0].second % 2 == 1)
            {
                nodes.insert(nodes.begin(), {smallest_2[0].first, 1, true});
            }

            length += newOccur * total_nodes;
        }
        // Else we combine the picked node/root into a new root
        else
        {
            // Push back the updated picked nodes/roots
            for (size_t i = 0; i < 2; ++i)
            {
                if (!smallest_2[i].is_tree && smallest_2[i].second != 1)
                {
                    --smallest_2[i].second;
                    nodes.insert(nodes.begin(), smallest_2[i]);
                }
            }


            for (size_t i = 0; i < 2; ++i)
            {
                newOccur += smallest_2[i].first;
                newNodesAmount += (smallest_2[i].is_tree) ? smallest_2[i].second : smallest_2[i].first;
            }

            length += newNodesAmount;

            // Insert the new root back into the vector, which maintains the ascending order
            NodesIter iter_nodes = std::lower_bound(nodes.begin(), nodes.end(), newOccur, [](const NodeSum &a, const size_t &b) { return a.first < b; });
            nodes.insert(iter_nodes, {newOccur, newNodesAmount, false, false, true});
        }

        // The condition when calculation is over
        if (nodes.size() == 1)
        {
            if (nodes.front().is_tree) { break; }
            if (nodes.front().is_tree_sum && nodes.front().second == 1) { break; }
        }
    }
    while (true);

    return length;
}

#endif // HUFFMAN_CALCULATOR_HPP
