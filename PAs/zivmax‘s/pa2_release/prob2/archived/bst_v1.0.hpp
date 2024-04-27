#ifndef CS10123F_PA2_BST
#define CS10123F_PA2_BST

#include <algorithm> // For std::max
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

class ScapegoatTree
{
        struct Node;

    public:
        ScapegoatTree() : root(nullptr), treeSize(0), maxSize(0) { }

        void insert(int data)
        {
            if (!root)
            {
                root = std::make_shared<Node>(data);
                treeSize++;
                maxSize = treeSize;
                return;
            }
            if (insert(root, data, 0))
            {
                maxSize = treeSize;
            }
        }

        std::shared_ptr<Node> search(int data)
        {
            std::shared_ptr<Node> current = root;
            while (current)
            {
                if (data == current->data)
                {
                    return current;
                }
                else if (data < current->data)
                {
                    current = current->left;
                }
                else
                {
                    current = current->right;
                }
            }
            return nullptr;
        }

        void erase(int data)
        {
            erase(root, data);
        }

    private:
        std::shared_ptr<Node> root;
        size_t treeSize;                     // Number of nodes in the tree
        size_t maxSize;                      // Maximum number of nodes before last rebuild
        static constexpr double alpha = 0.5; // Balance factor

        struct Node
        {
                int data;
                int subtreeSize; // size of the subtree taking the node itself as root
                std::shared_ptr<Node> left;
                std::shared_ptr<Node> right;
                Node(int val) : data(val), subtreeSize(1) { }
        };

        bool insert(std::shared_ptr<Node> &node, int data, int depth)
        {
            if (!node)
            {
                node = std::make_shared<Node>(data);
                treeSize++;
                return depth > log_alpha(treeSize); // Use a logarithmic depth check based on alpha
            }
            bool rebuild = false;
            if (data < node->data)
            {
                rebuild = insert(node->left, data, depth + 1);
            }
            else if (data > node->data)
            {
                rebuild = insert(node->right, data, depth + 1);
            }
            update_size(node); // Update the size of the current node
            if (rebuild)
            {
                if (size_of_subtree(node->left) > alpha * treeSize || size_of_subtree(node->right) > alpha * treeSize)
                {
                    rebuild_tree(node);
                    maxSize = treeSize;
                }
            }
            return rebuild;
        }

        void erase(std::shared_ptr<Node> &node, int data)
        {
            if (!node) return;

            // Perform standard BST delete
            node = delete_node(node, data);

            // Update size and possibly rebuild the tree
            if (node)
            {
                update_size(node);
                if (treeSize <= alpha * maxSize)
                {
                    rebuild_tree(root);
                    maxSize = treeSize;
                }
            }
            else
            {
                treeSize--;
            }
        }


        // Utility functions
        void update_size(std::shared_ptr<Node> &node)
        {
            if (!node) { return; }
            node->subtreeSize = 1 + size_of_subtree(node->left) + size_of_subtree(node->right);
        }

        int size_of_subtree(const std::shared_ptr<Node> &node)
        {
            return node ? node->subtreeSize : 0;
        }

        void flatten_tree(const std::shared_ptr<Node> &node, std::vector<std::shared_ptr<Node>> &nodes)
        {
            if (!node) { return; }
            flatten_tree(node->left, nodes);
            nodes.push_back(node);
            flatten_tree(node->right, nodes);
            // Reset the size since we are going to rebuild the subtree
            node->subtreeSize = 1;
        }

        std::shared_ptr<Node> rebuild_balance_from_sorted(std::vector<std::shared_ptr<Node>> &nodes, int start, int end)
        {
            if (start > end) { return nullptr; }
            int mid = start + (end - start) / 2;
            std::shared_ptr<Node> node = nodes[mid];
            node->left = rebuild_balance_from_sorted(nodes, start, mid - 1);
            node->right = rebuild_balance_from_sorted(nodes, mid + 1, end);
            update_size(node); // Update the size after subtrees are connected
            return node;
        }

        void rebuild_tree(std::shared_ptr<Node> &node)
        {
            std::vector<std::shared_ptr<Node>> nodes;
            flatten_tree(node, nodes);
            node = rebuild_balance_from_sorted(nodes, 0, nodes.size() - 1);
        }

        inline double log_alpha(int n)
        {
            return std::log(n) / std::log(1.0 / alpha);
        }

        std::shared_ptr<Node> find_min_node(std::shared_ptr<Node> node)
        {
            while (node && node->left)
            {
                node = node->left;
            }
            return node;
        }

        std::shared_ptr<Node> &delete_node(std::shared_ptr<Node> &node, int data)
        {
            if (!node) return node;

            if (data < node->data)
            {
                node->left = delete_node(node->left, data);
            }
            else if (data > node->data)
            {
                node->right = delete_node(node->right, data);
            }
            else
            {
                // Node with only one child or no child
                if (!node->left)
                {
                    return node->right;
                }
                else if (!node->right)
                {
                    return node->left;
                }

                // Node with two children: Get the in-order successor (smallest in the right subtree)
                std::shared_ptr<Node> temp = find_min_node(node->right);

                // Copy the in-order successor's content to this node
                node->data = temp->data;

                // Delete the in-order successor
                node->right = delete_node(node->right, temp->data);
            }
            return node;
        }
};

[[maybe_unused]] size_t operationCount = 0;
ScapegoatTree *tree;

// data structure initialization in this function
void init(std::size_t n [[maybe_unused]])
{
    operationCount = n;
    tree = new ScapegoatTree();
}

// deconstruct data structure in this function
void clear()
{
    delete tree;
}

// ... [Previous ScapegoatTree class definition] ...


// opt==1 insert x
// opt==2 erase x
// opt==3 query x
// opt==4 query xth
// opt==5 query x*sum(s1...st) - sum(s_t+1...s_m)
long long SetOperation(std::size_t opt, long long x)
{
    switch (opt)
    {
        case 1:
            tree->insert(x);
            break;

        case 2:
            tree->erase(x);
            break;

        case 3:
            return tree->search(x) ? 1 : 0;

        case 4:
            return 0; // TODO: Implement this

        case 5:
            return 0; // TODO: Implement this
        default:
            break;
    }
    return 0;
}


#endif // CS10123F_PA2_BST