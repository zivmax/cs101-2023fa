#ifndef CS10123F_PA2_BST
#define CS10123F_PA2_BST

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits.h>

class BST
{
private:
    struct Node
    {
        long long value;
        Node *left, *right;
        size_t size;
        long long sum;
        long long count;
        Node(long long val) : value(val), left(nullptr), right(nullptr), size(1), sum(val), count(1) {}
    };

    Node *root;
    std::vector<Node *> nodeList;
    const double alpha = 0.75;

    size_t getSize(Node *node) const
    {
        return node ? node->size : 0;
    }

    long long getSum(Node *node) const
    {
        return node ? node->sum : 0;
    }

    void updateNode(Node *node)
    {
        if (node)
        {
            node->size = getSize(node->left) + node->count + getSize(node->right);
            node->sum = getSum(node->left) + node->count * node->value + getSum(node->right);
        }
    }

    Node *insert(Node *node, long long value)
    {
        if (!node)
        {
            nodeList.push_back(new Node(value));
            return nodeList.back();
        }

        Node *&temp = (value < node->value) ? node->left : node->right;
        if (value != node->value)
        {
            temp = insert(temp, value);
        }
        else
        {
            ++node->count;
        }
        updateNode(node);
        return node;
    }

    Node *erase(Node *node, long long value)
    {
        if (node == nullptr)
        {
            return node;
        }
        Node *&find = (value < node->value) ? node->left : node->right;
        if (value != node->value)
        {
            find = erase(find, value);
        }
        else
        {
            if (node->count == 1)
            {
                if (!node->left || !node->right)
                {
                    Node *temp = node->left ? node->left : node->right;
                    delete node;
                    return temp;
                }
                else
                {
                    Node *current = node->right;
                    while (current && current->left)
                    {
                        current = current->left;
                    }
                    node->value = current->value;
                    node->right = erase(node->right, current->value);
                }
            }
            else
            {
                --node->count;
            }
        }

        updateNode(node);
        return node;
    }

    bool find(Node *node, long long value) const
    {
        while (node)
        {
            if (node->value == value)
            {
                return true;
            }
            node = value < node->value ? node->left : node->right;
        }
        return false;
    }

    long long kth(Node *node, size_t k) const
    {
        size_t leftSize = getSize(node->left);
        if (k == leftSize + 1)
        {
            return node->value;
        }
        else if (k <= leftSize)
        {
            return kth(node->left, k);
        }
        else
        {
            return kth(node->right, k - leftSize - 1);
        }
    }

    Node *findScapegoat(Node *node)
    {
        if (node == nullptr)
            return nullptr;
        if (getSize(node->left) > alpha * getSize(node) || getSize(node->right) > alpha * getSize(node))
        {
            return node;
        }
        if (getSize(node->left) > getSize(node->right))
        {
            return findScapegoat(node->left);
        }
        else
        {
            return findScapegoat(node->right);
        }
    }

    void Judge()
    {
        Node *scapegoat = findScapegoat(root);
        if (scapegoat != nullptr)
        {   

            nodeList.clear();
            compress(scapegoat);
            if (scapegoat == root)
            {
                root = rebuildTree(0, nodeList.size() - 1);
            }
            else
            {
                Node *parent = findParent(root, scapegoat->value);
                if (parent->left == scapegoat)
                {
                    parent->left = rebuildTree(0, nodeList.size() - 1);
                }
                else
                {
                    parent->right = rebuildTree(0, nodeList.size() - 1);
                }
            }
        }
    }

    Node *findParent(Node *node, long long value)
    {
        if (node == nullptr || node->value == value)
        {
            return nullptr;
        }
        if ((node->left != nullptr && node->left->value == value) || (node->right != nullptr && node->right->value == value))
        {
            return node;
        }
        if (value < node->value)
        {
            return findParent(node->left, value);
        }
        else
        {
            return findParent(node->right, value);
        }
    }

    Node *rebuildTree(size_t l, size_t r)
    {
        if (l > r)
        {
            return nullptr;
        }
        size_t mid = (l + r) / 2;
        Node *node = nodeList[mid];

        if (l == mid)
        {
            node->left = nullptr;
        }
        else
        {
            node->left = rebuildTree(l, mid - 1);
        }

        if (r == mid)
        {
            node->right = nullptr;
        }
        else
        {
            node->right = rebuildTree(mid + 1, r);
        }
        updateNode(node);
        return node;
    }

    void compress(Node *node)
    {
        if (!node)
        {
            return;
        }
        compress(node->left);
        nodeList.push_back(node);
        compress(node->right);
    }

    void clearTree(Node *node)
    {
        if (!node)
        {
            return;
        }
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }

    long long ans = LONG_MAX;

    void kSum(Node *node, long long k, long long pre = 0)
    {
        if (node)
        {
            long long total = getSum(root);
            long long left = node->left ? (getSum(node->left) + pre) : pre;
            long long k_left = k * left;
            long long right = total - left;
            if (k_left >= right)
            {
                if (node->left)
                {
                    ans = std::min(ans, k_left - right);
                    kSum(node->left, k, pre);
                }
            }
            else
            {
                k_left += k * node->value;
                right = total - left - node->value;
                if (k_left >= right)
                {
                    ans = k_left - right;
                }
                else if (node->right)
                {
                    pre += getSum(node->left) + node->value;
                    kSum(node->right, k, pre);
                }
            }
        }
    }

public:
    BST() : root(nullptr) {}

    void insert(long long value)
    {
        root = insert(root, value);
        Judge();
        // Node *parent = nullptr;
        //  root = insert(root, value, parent);
        // Judge(root, parent);
    }

    void erase(long long value)
    {
        root = erase(root, value);
        // Judge();
    }

    bool find(long long value)
    {
        return find(root, value);
    }

    long long kth(size_t k)
    {
        return kth(root, k);
    }

    long long kSum(size_t k)
    {
        if (!root)
        {
            return -1;
        }

        kSum(root, k, 0);
        return ans;
    }

    ~BST()
    {
        clear();
    }

    void clear()
    {
        clearTree(root);
        root = nullptr;
    }
};

BST bst;

void init(std::size_t n [[maybe_unused]]) {}

void clear()
{
    bst.clear();
}

long long SetOperation(std::size_t opt, long long x)
{
    switch (opt)
    {
    case 1:
        bst.insert(x);
        return 0;
    case 2:
        bst.erase(x);
        return 0;
    case 3:
        return bst.find(x) ? 1 : 0;
    case 4:
        return bst.kth(x);
    case 5:
        return bst.kSum(x);
    default:
        return 0;
    }
}

#endif // CS10123F_PA2_BST