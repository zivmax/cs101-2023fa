#ifndef CS10123F_PA2_BST
#define CS10123F_PA2_BST

#include <iostream>
#include <memory>
#include <vector>

template <typename T>
class Stack
{
    private:
        std::vector<T> elements;

    public:
        void push(const T &element) { elements.push_back(element); }

        void pop()
        {
            if (!elements.empty()) { elements.pop_back(); }
        }

        T top() const { return (!elements.empty()) ? elements.back() : nullptr; }

        bool empty() const { return elements.empty(); }

        size_t size() const { return elements.size(); }

        void clear() { elements.clear(); }
};

class ScapegoatTree
{
        struct Node;
        using DataType = size_t;
        friend void k_sum(const long long k, ScapegoatTree::Node *node, long long previous_left);
        friend long long SetOperation(std::size_t opt, long long x);


    public:
        ScapegoatTree() : root(nullptr), treeSize(0), maxSize(0) { }

        void insert(DataType data)
        {
            potentialGoats.clear();
            insert(root, data);
        }

        Node *search(DataType data)
        {
            Node *current = root;
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

        void erase(DataType data) { erase(root, data); }

        DataType query_data(size_t kth) { return query_data(root, kth); }

        size_t size() { return root->subtreeSize; }

        void print_tree() { print_tree(root, 0); }

        ~ScapegoatTree() { clear_tree(root); }

    private:
        Node *root;
        size_t treeSize;                      // Number of nodes in the tree
        size_t maxSize;                       // Maximum number of nodes before last rebuild
        static constexpr double alpha = 0.75; // Balance factor
        Stack<Node *> potentialGoats;

        struct Node
        {
                DataType data;
                size_t subtreeDataSum; // data sum of the subtree taking the node itself as root
                size_t subtreeSize;    // size of the subtree taking the node itself as root
                Node *left = nullptr;
                Node *right = nullptr;
                Node *parent = nullptr;
                Node(size_t val, Node *Parent) : data(val), subtreeDataSum(data), subtreeSize(1), parent(Parent) { }
        };

        void insert(Node *&node, DataType data)
        {
            if (!node)
            {
                node = new Node(data, potentialGoats.top());
                treeSize++;
                Node *scapegoat = find_scapegoat();
                if (scapegoat) { rebuild_subtree(scapegoat); }
                maxSize = std::max(treeSize, maxSize);
                return;
            }

            potentialGoats.push(node); // Track the ancestor before going deeper
            if (data <= node->data)
            {
                insert(node->left, data); // Pass the left child by reference
            }
            else if (data > node->data)
            {
                insert(node->right, data); // Pass the right child by reference
            }

            // After insertion, update the size of the current node's subtree.
            update_node(node);
        }


        void erase(Node *node, DataType data)
        {
            if (!node) return;

            // For root
            if (node == root && node->data == data)
            {
                if (node->left == nullptr && node->right == nullptr)
                {
                    delete node;
                    root = nullptr;
                    treeSize--;
                    maxSize = std::max(treeSize, maxSize);
                    return;
                }
                else if (node->left == nullptr)
                {
                    root = node->right;
                    root->parent = nullptr;
                    delete node;
                    treeSize--;
                    maxSize = std::max(treeSize, maxSize);
                    return;
                }
                else if (node->right == nullptr)
                {
                    root = node->left;
                    root->parent = nullptr;
                    delete node;
                    treeSize--;
                    maxSize = std::max(treeSize, maxSize);
                    return;
                }
            }

            // Perform standard BST delete
            node = delete_node(node, data);

            // Update size and possibly rebuild the tree
            if (node)
            {
                update_node(node);
                if (treeSize <= alpha * maxSize)
                {
                    rebuild_subtree(root);
                    maxSize = treeSize;
                }
            }
            else
            {
                treeSize--;
                maxSize = std::max(treeSize, maxSize);
            }
        }

        DataType query_data(Node *node, size_t kth)
        {
            if (!node) { return 0; }
            if (kth == size_of_subtree(node->left) + 1)
            {
                return node->data;
            }

            if (kth <= size_of_subtree(node->left))
            {
                return query_data(node->left, kth);
            }
            else
            {
                return query_data(node->right, kth - size_of_subtree(node->left) - 1);
            }
        }

        // Utility functions

        Node *find_scapegoat()
        {
            while (!potentialGoats.empty())
            {
                auto potentialGoat = potentialGoats.top();
                potentialGoats.pop();
                update_node(potentialGoat); // Ensure the size is up-to-date before checking
                if (size_of_subtree(potentialGoat->left) > alpha * potentialGoat->subtreeSize ||
                    size_of_subtree(potentialGoat->right) > alpha * potentialGoat->subtreeSize)
                {
                    return potentialGoat;
                }
            }
            return nullptr;
        }

        void rebuild_subtree(Node *node)
        {
            std::vector<Node *> nodes;
            flatten_tree(node, nodes);
            Node *parentOfNewRoot = node->parent;
            if (parentOfNewRoot != nullptr)
            {
                if (parentOfNewRoot->left == node)
                {
                    parentOfNewRoot->left = rebuild_bst_from_sorted(parentOfNewRoot, nodes, 0, nodes.size() - 1);
                }
                else
                {
                    parentOfNewRoot->right = rebuild_bst_from_sorted(parentOfNewRoot, nodes, 0, nodes.size() - 1);
                }
            }
            else
            {
                root = rebuild_bst_from_sorted(parentOfNewRoot, nodes, 0, nodes.size() - 1);
            }
        }

        void flatten_tree(Node *node, std::vector<Node *> &nodes)
        {
            if (!node) { return; }
            flatten_tree(node->left, nodes);
            nodes.push_back(node); // Add the current node to the vector
            flatten_tree(node->right, nodes);
            // Reset the size since we are going to rebuild the subtree
            node->subtreeSize = 1;
        }


        Node *rebuild_bst_from_sorted(Node *new_parent, std::vector<Node *> &nodes, int start, int end)
        {
            if (start > end) { return nullptr; }
            int mid = start + (end - start) / 2;
            Node *node = nodes[mid];
            node->parent = new_parent;
            node->left = rebuild_bst_from_sorted(node, nodes, start, mid - 1);
            node->right = rebuild_bst_from_sorted(node, nodes, mid + 1, end);
            update_node(node); // Update the size after subtrees are connected
            return node;
        }

        void update_node(Node *node)
        {
            if (!node) { return; }
            node->subtreeSize = 1 + size_of_subtree(node->left) + size_of_subtree(node->right);
            node->subtreeDataSum = node->data + data_sum_of_subtree(node->left) + data_sum_of_subtree(node->right);
        }

        size_t size_of_subtree(const Node *node) { return node ? node->subtreeSize : 0; }

        size_t data_sum_of_subtree(const Node *node) { return node ? node->subtreeDataSum : 0; }

        Node *find_min_node(Node *node)
        {
            while (node && node->left)
            {
                node = node->left;
            }
            return node;
        }

        Node *delete_node(Node *node, DataType data)
        {
            if (!node) return nullptr;

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
                    Node *temp = node->right;
                    // Update parent
                    if (temp) { temp->parent = node->parent; }
                    delete node; // Delete the current node
                    return temp; // Return the non-null child or nullptr if no children
                }
                else if (!node->right)
                {
                    Node *temp = node->left;
                    // Update parent
                    if (temp) { temp->parent = node->parent; }
                    delete node; // Delete the current node
                    return temp; // Return the non-null child
                }

                // Node with two children: Get the in-order successor (smallest in the right subtree)
                Node *temp = find_min_node(node->right);

                // Copy the in-order successor's content to this node
                node->data = temp->data;

                // Delete the in-order successor
                node->right = delete_node(node->right, temp->data);
            }

            // If the node had two children, we update its size after swapping with the in-order successor
            update_node(node);
            return node;
        }

        void print_tree(const Node *node, int depth)
        {
            if (!node)
            {
                return;
            }
            print_tree(node->right, depth + 1);
            std::cout << std::string(depth * 4, ' ') << node->data << " (" << node->subtreeSize << ", " << node->subtreeDataSum << ")" << std::endl;
            print_tree(node->left, depth + 1);
        }

        void clear_tree(Node *node)
        {
            if (node)
            {
                clear_tree(node->left);
                clear_tree(node->right);
                delete node;
            }
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


long long closestSum = LLONG_MAX;
size_t leftSumCache;
size_t rightSumCache;


void k_sum(const long long k, ScapegoatTree::Node *node, long long accumulated_left_sum)
{
    if (!node) 
    {
        return;
    }
    long long nodeValue = node->data; 
    long long sumToLeft = accumulated_left_sum; 

    sumToLeft += (node->left ? node->left->subtreeDataSum : 0); // Add the sum of the left subtree to sumToLeft.

    // Calculate the sum of all elements to the right of the current node.
    long long sumToRight = tree->root->subtreeDataSum - sumToLeft;
    long long kLeftSum = sumToLeft * k; 

    if (kLeftSum >= sumToRight) 
    {
        if (node->left)
        {
            closestSum = std::min(closestSum, kLeftSum - sumToRight); // Update closestSum with the minimum difference.
            k_sum(k, node->left, accumulated_left_sum); // Recursive call to explore the left subtree.
        }
    }
    else // If k times the left sum is less than the right sum, explore the right subtree.
    {
        sumToRight -= nodeValue; 
        kLeftSum += nodeValue * k; 
        if (kLeftSum < sumToRight) 
        {
            // Update the accumulated left sum to include the current node and its left subtree.
            accumulated_left_sum += (node->left ? node->left->subtreeDataSum : 0) + nodeValue;
            k_sum(k, node->right, accumulated_left_sum); 
        }
        else
        {
            closestSum = std::min(closestSum, kLeftSum - sumToRight); 
            return; 
        }
    }
}


long long SetOperation(std::size_t opt, long long x)
{
    std::vector<ScapegoatTree::Node *> Nodes;

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
            return tree->query_data(x);

        case 5:
            leftSumCache = 0;
            rightSumCache = tree->data_sum_of_subtree(tree->root);

            k_sum(x, tree->root, leftSumCache);
            return closestSum;
        default:
            break;
    }
    return 0;
}


#endif // CS10123F_PA2_BST