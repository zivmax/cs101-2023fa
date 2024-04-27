#ifndef HUFFMAN_CALCULATOR_HPP
#define HUFFMAN_CALCULATOR_HPP

#include <tuple>
#include <vector>
#include <complex>

class Node
{
private:
public:
    size_t frequency;
    size_t amount = 0;
    size_t sum = 0;
    Node() = default;
    Node(const std::pair<size_t, size_t> data) : frequency(data.first), amount(data.second) {}
    size_t get_sum() const
    {
        if(sum == 0)
            return frequency;
        else
            return sum;
        return sum;
    }
};

class Heap
{
private:
    std::vector<Node> heap;
    int size = 0;
public:
    Heap(const std::vector<std::pair<size_t, size_t>>& data)
    {
        for(auto &it : data)
        {
            push(Node(it));
        }
    }
    void push(Node node)
    {
        heap.push_back(node);
        size += 1;
        int pos = size - 1;
        while(pos > 0)
        {
            int parent = (pos - 1) / 2;
            if(heap[parent].frequency > heap[pos].frequency)
            {
                std::swap(heap[parent], heap[pos]);
                pos = parent;
            }else{
                break;
            }
        }
    }
    Node pop()
    {
        Node ret = heap[0];
        std::swap(heap[0], heap[size - 1]);
        heap.pop_back();
        size -= 1;
        int pos = 0;
        while(pos < size)
        {
            int l = 2 * pos + 1;
            int r = 2 * pos + 2;
            int min = pos;
            if(l < size && heap[l].frequency < heap[min].frequency)
                min = l;
            if(r < size && heap[r].frequency < heap[min].frequency)
                min = r;
            if(pos != min)
            {
                std::swap(heap[pos], heap[min]);
                pos = min;
            }else{
                break;
            }
        }
        return ret;
    }
    bool is_empty() const
    {
        return size == 0;
    }
    int get_size() const
    {
        return size;
    }
};

size_t get_huffman_length(const std::vector<std::pair<size_t, size_t>> &data) {
    size_t length = 0;
    Heap heap(data);
    while(1)
    {
        Node node1 = heap.pop();
            if(node1.amount != 1)
            {
                if(node1.amount % 2 == 1)
                {
                    heap.push(Node(std::make_pair(node1.frequency, 1)));
                }
                node1.frequency *= 2;
                node1.amount /= 2;
                heap.push(node1);
                length += node1.frequency * node1.amount;
            }
            else if(heap.is_empty())
                break;
            else
            {
                Node node2 = heap.pop();
                Node node3 = Node(std::make_pair(node1.frequency + node2.frequency, 1));
                node3.sum = node1.get_sum() + node2.get_sum();
                if(node2.amount != 1)
                {
                    node2.amount--;
                    heap.push(node2);
                }
                heap.push(node3);
                length += node3.sum;
            }
    }
  return length;
}
#endif // HUFFMAN_CALCULATOR_HPP