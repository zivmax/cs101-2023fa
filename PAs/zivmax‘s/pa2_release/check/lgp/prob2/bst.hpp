#ifndef CS10123F_PA2_BST
#define CS10123F_PA2_BST

#include <utility>
#include<vector>
#include<iostream>
#include<memory>

class Node
{
public:
    long long value;
    std::size_t size = 1;
    long long sum;
    std::size_t cnt = 1;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    Node(long long &value) : value(value), sum(value) {}

};

class BST
{
public:
    std::shared_ptr<Node> root = nullptr;
    std::vector<std::shared_ptr<Node>> nodes;
    const double alpha = 0.75;
    BST() = default;
    void update_node(std::shared_ptr<Node> node)
    {
            node->size = node->cnt;
            node->size += node->left ? node->left->size : 0;
            node->size += node->right ? node->right->size : 0;
            node->sum = node->cnt * node->value;
            node->sum += node->left ? node->left->sum : 0;
            node->sum += node->right ? node->right->sum : 0;
    }
//    bool check_rebuild(Node *cur)
//    {
//        if (cur->size * alpha < cur->left->size || cur->size * alpha < cur->right->size)
//        {
//            std::vector<long long> values;
//            inorder(cur, values);
//            cur = build(values, 0, values.size() - 1, nullptr);
//        }
//    }
    void insert(long long value)
    {
        if(root == nullptr)
        {
            root = std::make_shared<Node>(value);
            return;
        }
        std::shared_ptr<Node> cur = root;
        nodes.push_back(root);
        while(true)
        {
            if(value < cur->value)
            {
                if(cur->left == nullptr)
                {
                    cur->left = std::make_shared<Node>(value);
                    break;
                }
                else
                    cur = cur->left;
            }
            else if(value > cur->value)
            {
                if(cur->right == nullptr)
                {
                    cur->right = std::make_shared<Node>(value);
                    break;
                }
                else
                    cur = cur->right;
            }
            else
            {
                cur->cnt++;
                break;
            }
            nodes.push_back(cur);
        }
        for(auto it = nodes.rbegin(); it != nodes.rend(); it++)
            update_node(*it);
        nodes.clear();
    }

    void erase(long long value)
    {
        std::shared_ptr<Node> cur = root;
        while (cur != nullptr)
        {
            nodes.push_back(cur);
            if(value < cur->value)
                cur = cur->left;
            else if(value > cur->value)
                cur = cur->right;
            else
            {
                if(cur->cnt > 1)
                    cur->cnt--;
                else if(cur->left == nullptr || cur->right == nullptr)
                {
                    std::shared_ptr<Node> tmp = cur->left ? cur->left : cur->right;
                    if(cur == root)
                        root = tmp;
                    else
                    {
                        auto it = nodes.rbegin() + 1;
                        (*it)->left == cur ? (*it)->left = tmp : (*it)->right = tmp;
                    }
                }
                else
                {
                    std::shared_ptr<Node> min = cur->right;
                    std::shared_ptr<Node> minParent = nullptr;

                    while(min->left)
                    {
                        nodes.push_back(min);
                        minParent = min;
                        min = min->left;
                    }

                    cur->value = min->value;
                    cur->cnt = min->cnt;
                    if(minParent)
                        minParent->left = min->right;
                    else
                        cur->right = min->right;
                }
                break;
            }
        }
        for(auto it = nodes.rbegin(); it != nodes.rend(); it++)
        {
            update_node(*it);
        }
        nodes.clear();
    }

    void inorder(const std::shared_ptr<Node>& node)
    {
        if(!node)
            return;
        inorder(node->left);
        nodes.push_back(node);
        inorder(node->right);
    }

    std::shared_ptr<Node> pick_goat(std::shared_ptr<Node> node)
    {
        if(!node)
            return nullptr;
        if(node->left && node->left->size > alpha * node->size)
            return node;
        else if(node->right && node->right->size > alpha * node->size)
            return node;
        if(node->left && node->right && node->left->size > node->right->size)
            return pick_goat(node->left);
        else if(node->left && node->right && node->left->size < node->right->size)
            return pick_goat(node->right);
        return nullptr;
    }

    std::shared_ptr<Node> rebuild(size_t left, size_t right)
    {
        if(left > right)
            return nullptr;
        size_t mid = (left + right) / 2;
        std::shared_ptr<Node> node = nodes.at(mid);
        if(left == mid)
            node->left = nullptr;
        else
        {
            node->left = rebuild(left, mid - 1);
        }
        if(right == mid)
            node->right = nullptr;
        else
        {
            node->right = rebuild(mid + 1, right);
        }
        update_node(node);
        return node;
    }

    void check_rebuild()
    {
        std::shared_ptr<Node> goat = pick_goat(root);
        if(goat == nullptr)
            return;
        else
        {
            inorder(goat);
            if(goat == root)
                root = rebuild(0, nodes.size() - 1);
            else
            {
                std::shared_ptr<Node> parent = root;
                while(true)
                {
                    if(parent->left == goat || parent->right == goat)
                    {
                        break;
                    }
                    else
                    {
                        if(goat->value < parent->value)
                            parent = parent->left;
                        else
                            parent = parent->right;
                    }
                }
                parent->left == goat ? parent->left = rebuild(0, nodes.size() - 1) : parent->right = rebuild(0, nodes.size() - 1);
            }
        }
        nodes.clear();
    }

    bool query(long long value)
    {
        std::shared_ptr<Node> cur = root;
        while(cur != nullptr)
        {
            if(value < cur->value)
                cur = cur->left;
            else if(value > cur->value)
                cur = cur->right;
            else
                return true;
        }
        return false;
    }

    long long get_kth(long long k)
    {
        std::shared_ptr<Node> cur = root;
        while(cur != nullptr)
        {
            int left_size = cur->left ? cur->left->size : 0;
            if(cur->left && cur->left->size >= k)
                cur = cur->left;
            else if(left_size + cur->cnt >= k)
                return cur->value;
            else
            {
                k -= cur->left ? cur->left->size : 0;
                k -= cur->cnt;
                cur = cur->right;
            }
        }
        return 0;
    }

    long long min_sum(long long k)
    {
        std::shared_ptr<Node> cur = root;
        long long root_sum = cur->sum;
        long long pre_sum = 0;
        long long min = LONG_LONG_MAX;
        while(cur)
        {
            long long front_sum = cur->left ? (cur->left->sum + pre_sum) : pre_sum;
            long long back_sum = root_sum - front_sum;
            if(k * front_sum >= back_sum)
            {
                if(cur->left)
                {
                    min = std::min(min, k * front_sum - back_sum);
                    cur = cur->left;
                }
                else
                    break;
            }
            else
            {
                front_sum += cur->value;
                back_sum  -= cur->value;
                if(k * front_sum >= back_sum)
                {
                    min = std::min(min, k * front_sum - back_sum);
                    break;
                }
                else if(cur->right)
                {
                    pre_sum = front_sum;
                    cur = cur->right;
                }
                else
                    break;
            }
        }

        return min;
    }

    void clear()
    {
        nodes.clear();
        root = nullptr;
    }
};

BST bst;


//you can do data structure initialization in this function
void init(std::size_t n [[maybe_unused]])
{
}

//You can deconstruct your data structure in this function
void clear()
{
    bst.clear();
}

// opt==1 insert x
// opt==2 erase x
// opt==3 query x
// opt==4 query xth
// opt==5 query x*sum(s1...st) - sum(s_t+1...s_m)
long long SetOperation(std::size_t opt, long long x)
{
    switch (opt) {
        case 1:
            bst.insert(x);
            bst.check_rebuild();
            break;
        case 2:
            bst.erase(x);
            break;
        case 3:
            return bst.query(x);
        case 4:
            return bst.get_kth(x);
        case 5:
            return bst.min_sum(x);
        default:
            return 0;
    }
    return 0;
}



#endif //CS10123F_PA2_BST