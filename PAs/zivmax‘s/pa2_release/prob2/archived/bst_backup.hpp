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

template <typename T>
class Queue
{
    private:
        std::vector<T> elements;

    public:
        // Enqueue an element at the back of the queue
        void push(const T &element) { elements.push_back(element); }

        // Dequeue an element from the front of the queue
        void pop()
        {
            if (!elements.empty()) { elements.erase(elements.begin()); }
        }

        // Get the front element of the queue
        T front() const
        {
            if (!elements.empty()) { return elements.front(); }
            throw std::out_of_range("Queue is empty");
        }

        // Check if the queue is empty
        bool empty() const { return elements.empty(); }

        // Get the number of elements in the queue
        size_t size() const { return elements.size(); }

        // Clear all elements from the queue
        void clear() { elements.clear(); }
};

class ScapegoatTree
{
        struct Node;
        using DataType = size_t;
        using NodePtr = std::shared_ptr<Node>;
        friend size_t k_sum(NodePtr node, size_t k);
        friend size_t k_sum(std::vector<ScapegoatTree::NodePtr> &nodes, size_t k);
        friend long long SetOperation(std::size_t opt, long long x);


    public:
        ScapegoatTree() : root(nullptr), treeSize(0), maxSize(0) { }

        void insert(DataType data)
        {
            potentialGoats.clear();
            insert(root, data);
        }

        NodePtr search(DataType data)
        {
            NodePtr current = root;
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

        void print_tree()
        {
            // If not empty
            if (root)
                print_tree(root, 0);
            else
                std::cout << "Empty tree" << std::endl;
        }

    private:
        NodePtr root;
        size_t treeSize;                      // Number of nodes in the tree
        size_t maxSize;                       // Maximum number of nodes before last rebuild
        static constexpr double alpha = 0.75; // Balance factor
        Queue<NodePtr> potentialGoats;

        struct Node
        {
                DataType data;
                size_t subtreeDataSum; // data sum of the subtree taking the node itself as root
                size_t subtreeSize;    // size of the subtree taking the node itself as root
                NodePtr left;
                NodePtr right;
                Node(size_t val) : data(val), subtreeDataSum(data), subtreeSize(1) { }
        };

        void insert(NodePtr &node, DataType data)
        {
            if (!node)
            {
                node = std::make_shared<Node>(data);
                treeSize++;
                
                if (check_unbalanced(root))
                {
                    rebuild_child_subtree(nullptr);
                }
                // First check if whole tree should be rebuilt
                NodePtr scapegoatParent = find_scapegoat();
                if (scapegoatParent) { rebuild_child_subtree(scapegoatParent); }
                maxSize = std::max(treeSize, maxSize);
                return;
            }

            potentialGoats.push(node); // Track the ancestor before going deeper
            if (data <= node->data)
            {
                insert(node->left, data);
            }
            else if (data > node->data)
            {
                insert(node->right, data);
            }
            update_node(node);
        }

        void erase(NodePtr &node, DataType data)
        {
            if (!node) return;

            // Perform standard BST delete
            node = delete_node(node, data);

            // Update size and possibly rebuild the tree
            if (node)
            {
                update_node(node);
                if (treeSize <= alpha * maxSize)
                {
                    rebuild_child_subtree(root);
                    maxSize = treeSize;
                }
            }
            else
            {
                treeSize--;
                maxSize = std::max(treeSize, maxSize);
            }
        }

        DataType query_data(NodePtr node, size_t kth)
        {
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

        NodePtr query_node(NodePtr node, size_t kth)
        {
            if (kth == size_of_subtree(node->left) + 1)
            {
                return node;
            }

            if (kth <= size_of_subtree(node->left))
            {
                return query_node(node->left, kth);
            }
            else
            {
                return query_node(node->right, kth - size_of_subtree(node->left) - 1);
            }
        }

        // Utility functions

        bool check_unbalanced(NodePtr &node)
        {
            if (!node) { return false; }
            return size_of_subtree(node->left) > alpha * node->subtreeSize ||
                   size_of_subtree(node->right) > alpha * node->subtreeSize;
        }

        NodePtr find_scapegoat()
        {
            while (!potentialGoats.empty())
            {
                auto potentialGoatParent = potentialGoats.front();
                potentialGoats.pop();
                update_node(potentialGoatParent); // Ensure the size is up-to-date before checking
                if (check_unbalanced(potentialGoatParent->left) || check_unbalanced(potentialGoatParent->right))
                {
                    return potentialGoatParent;
                }
            }
            return nullptr;
        }

        void rebuild_child_subtree(NodePtr scapegoarParent)
        {
            std::vector<NodePtr> nodes;
            if (scapegoarParent == nullptr)
            {
                flatten_tree(root, nodes);
                root = rebuild_bst_from_sorted(nodes, 0, nodes.size() - 1);
            }
            else if (check_unbalanced(scapegoarParent->left))
            {
                flatten_tree(scapegoarParent->left, nodes);
                scapegoarParent->left = rebuild_bst_from_sorted(nodes, 0, nodes.size() - 1);
            }
            else if (check_unbalanced(scapegoarParent->right))
            {
                flatten_tree(scapegoarParent->right, nodes);
                scapegoarParent->right = rebuild_bst_from_sorted(nodes, 0, nodes.size() - 1);
            }
        }

        void flatten_tree(const NodePtr &node, std::vector<NodePtr> &nodes)
        {
            if (!node) { return; }
            flatten_tree(node->left, nodes);
            nodes.push_back(node);
            flatten_tree(node->right, nodes);
            // Reset the size since we are going to rebuild the subtree
            node->subtreeSize = 1;
        }

        NodePtr rebuild_bst_from_sorted(std::vector<NodePtr> &nodes, int start, int end)
        {
            if (start > end) { return nullptr; }
            int mid = start + (end - start) / 2;
            NodePtr node = nodes[mid];
            node->left = rebuild_bst_from_sorted(nodes, start, mid - 1);
            node->right = rebuild_bst_from_sorted(nodes, mid + 1, end);
            update_node(node); // Update the size after subtrees are connected
            return node;
        }

        void update_node(NodePtr &node)
        {
            if (!node) { return; }
            node->subtreeSize = 1 + size_of_subtree(node->left) + size_of_subtree(node->right);
            node->subtreeDataSum = node->data + data_sum_of_subtree(node->left) + data_sum_of_subtree(node->right);
        }

        static inline size_t size_of_subtree(const NodePtr &node) { return node ? node->subtreeSize : 0; }

        static inline size_t data_sum_of_subtree(const NodePtr &node) { return node ? node->subtreeDataSum : 0; }

        NodePtr find_min_node(NodePtr node)
        {
            while (node && node->left)
            {
                node = node->left;
            }
            return node;
        }

        NodePtr &delete_node(NodePtr &node, DataType data)
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
                NodePtr temp = find_min_node(node->right);

                // Copy the in-order successor's content to this node
                node->data = temp->data;

                // Delete the in-order successor
                node->right = delete_node(node->right, temp->data);

                update_node(node);
            }
            return node;
        }

        void print_tree(const NodePtr &node, int depth)
        {
            if (!node)
            {
                return;
            }
            print_tree(node->right, depth + 1);
            std::cout << std::string(depth * 4, ' ') << node->data << " (" << node->subtreeSize << ", " << node->subtreeDataSum << ")" << std::endl;
            print_tree(node->left, depth + 1);
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


size_t leftSumCache;
size_t rightSumCache;
size_t closestSum;


size_t k_sum(std::vector<ScapegoatTree::NodePtr> &nodes, size_t k)
{
    // Since the nodes flattened from the tree are sorted, we can use two pointers to find the closest sum
    size_t right = nodes.size();
    for (size_t i = 0; i < right; i++)
    {
        leftSumCache += nodes[i]->data;
        rightSumCache -= nodes[i]->data;

        if (k * leftSumCache >= rightSumCache)
        {
            return k * leftSumCache - rightSumCache;
        }
    }

    return 0;
}
long long SetOperation(std::size_t opt, long long x)
{
    std::vector<ScapegoatTree::NodePtr> nodes;

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
            closestSum = LONG_LONG_MAX;
            leftSumCache = 0;
            rightSumCache = ScapegoatTree::data_sum_of_subtree(tree->root);
            tree->flatten_tree(tree->root, nodes);
            return k_sum(nodes, x);
        default:
            break;
    }
    return 0;
}


#endif // CS10123F_PA2_BST