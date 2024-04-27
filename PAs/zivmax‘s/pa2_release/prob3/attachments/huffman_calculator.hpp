#ifndef HUFFMAN_CALCULATOR_HPP
#define HUFFMAN_CALCULATOR_HPP

#include <algorithm>
#include <memory>
#include <tuple>
#include <vector>

using std::vector;

// Multi Abstraction Data Type, for store different types of abstraction data in one container
struct MultiAbs
{
        size_t first;
        size_t second;
        bool is_node_sum = false; // if is_node_summary: {occurrence, amount of abstracts having that same occurrence}
        bool is_tree_sum = false; // if is_tree_sum: {occurrence summation, amount of perfect trees having that same occurrence summation}
        bool is_tree = false;     // if is_tree: {occurrence summation of every descendants of the root, the length will increase if the tree's height increases by 1}

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

        bool operator>(const MultiAbs &other) const
        {
            return first > other.first;
        }
};

class MinHeap
{
    private:
        std::vector<MultiAbs> heap;

        size_t parent(size_t i) { return (i - 1) / 2; }

        size_t left_child_index(size_t i) { return 2 * i + 1; }

        size_t right_child_index(size_t i) { return 2 * i + 2; }

        void sift_up(size_t i)
        {
            // While 'i' is not the root and the parent of 'i' is greater than 'i'
            while (i > 0 && heap[parent(i)] > heap[i])
            {
                // Swap the parent and the child
                std::swap(heap[parent(i)], heap[i]);

                i = parent(i);
            }
        }

        void sift_down(size_t i)
        {
            size_t minIndex = i;

            size_t left = left_child_index(i);

            // If the left child exists and is less than the element at 'minIndex', update 'minIndex' to the left child's index
            if (left < heap.size() && heap[left] < heap[minIndex])
            {
                minIndex = left;
            }

            size_t right = right_child_index(i);

            // If the right child exists and is less than the element at 'minIndex', update 'minIndex' to the right child's index
            if (right < heap.size() && heap[right] < heap[minIndex])
            {
                minIndex = right;
            }

            // If 'minIndex' is not 'i', it means one of the children is smaller than the parent.
            if (i != minIndex)
            {
                std::swap(heap[i], heap[minIndex]);
                sift_down(minIndex);
            }
        }

    public:
        void push(MultiAbs value)
        {
            heap.push_back(value);
            sift_up(heap.size() - 1);
        }

        MultiAbs pop()
        {
            MultiAbs result = heap[0];
            heap[0] = heap[heap.size() - 1];
            heap.pop_back();
            if (!heap.empty())
            {
                sift_down(0);
            }
            return result;
        }

        MultiAbs top() { return heap[0]; }

        bool empty() { return heap.size() == 0; }

        size_t size() { return heap.size(); }
};


using AbsIter = vector<MultiAbs>::iterator;

size_t get_huffman_length(const std::vector<std::pair<size_t, size_t>> &data)
{
    size_t length = 0;

    MinHeap abstracts;

    // Store data in abstracts first
    for (auto &i : data)
    {
        abstracts.push(i);
    }

    do
    {
        // Pick the 2 smallest element by `first`
        MultiAbs smallest_2[2];
        bool over_pick = false;

        for (int i = 0; i < 2; ++i)
        {
            if (!abstracts.empty())
            {
                smallest_2[i] = abstracts.pop();
            }
            else
            {
                smallest_2[i] = {0, 0, false, false, true};
                over_pick = true;
            }
        }

        size_t newOccur = 0;
        size_t newIncreaseLength = 0;

        // When the smallest is a trees or a node summary, we can handle it quickly.
        if (smallest_2[0].second != 1 && !smallest_2[0].is_tree)
        {
            // Push back the uselessly picked abstracts/roots
            if (!over_pick) { abstracts.push(smallest_2[1]); }

            // When the smallest is a node with corresponding node summary of more than 1, we can handle it quickly.
            newOccur = smallest_2[0].first * 2;
            size_t total_nodes = smallest_2[0].second / 2;

            // Insert the new trees back into the vector, which maintains the ascending order
            abstracts.push({newOccur, total_nodes, false, true});
            if (smallest_2[0].second % 2 == 1)
            {
                abstracts.push({smallest_2[0].first, 1, true});
            }

            length += newOccur * total_nodes;
        }
        // Else we combine the picked node/root into a new root
        else
        {
            // Push back the updated picked abstracts/roots
            for (size_t i = 0; i < 2; ++i)
            {
                if (!smallest_2[i].is_tree && smallest_2[i].second != 1)
                {
                    --smallest_2[i].second;
                    abstracts.push(smallest_2[i]);
                }
            }


            for (size_t i = 0; i < 2; ++i)
            {
                newOccur += smallest_2[i].first;
                newIncreaseLength += (smallest_2[i].is_tree) ? smallest_2[i].second : smallest_2[i].first;
            }

            length += newIncreaseLength;

            // Insert the new root back into the vector, which maintains the ascending order
            abstracts.push({newOccur, newIncreaseLength, false, false, true});
        }

        // The condition when calculation is over
        if (abstracts.size() == 1)
        {
            if (abstracts.top().is_tree) { break; }
            if (abstracts.top().is_tree_sum && abstracts.top().second == 1) { break; }
        }
    }
    while (true);

    return length;
}

#endif // HUFFMAN_CALCULATOR_HPP
