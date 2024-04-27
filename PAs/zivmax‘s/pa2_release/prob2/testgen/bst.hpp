#ifndef CS10123F_PA2_BST
#define CS10123F_PA2_BST

#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Node;
class ScapeGoat;
using Nodeptr = shared_ptr<Node>;

class Node
{
        friend ScapeGoat;

    protected:
        long long value;
        Nodeptr left_node = nullptr;
        Nodeptr right_node = nullptr;
        // size indicates the size of the subtree
        int size = 1;
        long long sub_sum;
        int same_count = 1;


        // long long sub_sum = 0;
    public:
        Nodeptr &get_left_node()
        {
            return this->left_node;
        }
        Nodeptr &get_right_node()
        {
            return this->right_node;
        }
        long long get_value() const
        {
            return this->value;
        }
        void set_left_node(Nodeptr l)
        {
            this->left_node = l;
        }
        void set_right_node(Nodeptr r)
        {
            this->right_node = r;
        }
        Node(long long Value) : value(Value), sub_sum(Value) { }
        Node() = default;
};

class ScapeGoat
{
    private:
        vector<Nodeptr> SGT;
        int maxNodeCnt = 0;
        Nodeptr root = nullptr;
        long long Delta = 100000000000;

        int get_node_size(Nodeptr current)
        {
            if (current)
            {
                return current->size;
            }
            return 0;
        }

        void inorder(Nodeptr current)
        {
            if (current == nullptr)
            {
                return;
            }
            inorder(current->get_left_node());

            if (current->same_count)
            {
                SGT.push_back(current);
            }
            inorder(current->get_right_node());
        }
        Nodeptr rebuild(int left, int right)
        {
            if (left > right)
            {
                return nullptr;
            }
            int mid = (left + right) / 2;
            SGT[mid]->get_left_node() = rebuild(left, mid - 1);
            SGT[mid]->get_right_node() = rebuild(mid + 1, right);
            // update the size
            SGT[mid]->size = get_node_size(SGT[mid]->get_left_node()) + get_node_size(SGT[mid]->get_right_node()) + SGT[mid]->same_count;
            // update the sub_sum
            SGT[mid]->sub_sum = SGT[mid]->get_value();
            if (SGT[mid]->get_left_node())
            {
                SGT[mid]->sub_sum += SGT[mid]->get_left_node()->sub_sum;
            }
            if (SGT[mid]->get_right_node())
            {
                SGT[mid]->sub_sum += SGT[mid]->get_right_node()->sub_sum;
            }

            return SGT[mid];
        }
        // return the imbalance node
        Nodeptr check_balance(Nodeptr current)
        {
            if (!current)
            {
                return nullptr;
            }
            int left_sub_size = get_node_size(current->get_left_node());
            int right_sub_size = get_node_size(current->get_right_node());

            // satisfies the condition
            if (get_node_size(current) * 0.75 < max(left_sub_size, right_sub_size))
            {
                return current;
            }
            if (right_sub_size > left_sub_size)
            {
                return check_balance(current->get_right_node());
            }
            else
            {
                return check_balance(current->get_left_node());
            }
        }

        void set_previous_node(long long val, Nodeptr current)
        {
            if (!current)
            {
                return;
            }
            int boundry = SGT.size() - 1;
            if (current->get_left_node() && current->get_left_node()->get_value() == val)
            {
                current->get_left_node() = rebuild(0, boundry);
                return;
            }
            else if (current->get_right_node() && current->get_right_node()->get_value() == val)
            {
                current->get_right_node() = rebuild(0, boundry);
                return;
            }
            else
            {
                if (val < current->get_value())
                {
                    set_previous_node(val, current->get_left_node());
                }
                else
                {
                    set_previous_node(val, current->get_right_node());
                }
            }
        }
        void remake()
        {
            auto imbalance = check_balance(root);
            if (!imbalance)
            {
                return;
            }
            SGT.clear();
            inorder(imbalance);
            int boundry = SGT.size() - 1;
            if (imbalance != root)
            {
                set_previous_node(imbalance->value, root);
            }
            else
            {
                root = rebuild(0, boundry);
            }
        }
        int exist(int same_count)
        {
            if (same_count > 0)
            {
                return 1;
            }
            return 0;
        }

        void print_tree(const Nodeptr &node, int depth)
        {
            if (!node)
            {
                return;
            }
            print_tree(node->right_node, depth + 1);
            std::cout << std::string(depth * 4, ' ') << node->value << " (" << node->size << ", " << node->sub_sum << ")" << std::endl;
            print_tree(node->left_node, depth + 1);
        }

    public:
        friend long long SetOperation(std::size_t opt, long long x);

        void print_tree() { print_tree(root, 0); }

        size_t size()
        {
            return root->size;
        }

        int insert(Nodeptr &current, const long long x)
        {
            if (current == nullptr)
            {
                // set leaves
                current = make_shared<Node>(x);
                this->maxNodeCnt++;
                SGT.push_back(current);
                if (check_balance(root))
                {
                    remake();
                }
                return 0;
            }
            else
            {
                current->size++;
                current->sub_sum += x;
                if (x > current->get_value())
                {
                    return insert(current->get_right_node(), x);
                }
                else if (x == current->get_value())
                {
                    current->same_count++;
                    return 0;
                }
                else
                {
                    return insert(current->get_left_node(), x);
                }
            }
        }

        int query(Nodeptr current, const long long x)
        {

            if (current == nullptr)
            {
                return 0;
            }
            else
            {
                if (x == current->get_value() && current->same_count)
                {
                    return 1;
                }
                if (x > current->get_value())
                {
                    return query(current->get_right_node(), x);
                }
                else
                {
                    return query(current->get_left_node(), x);
                }
            }
        }
        int erase(Nodeptr current, const long long x)
        {

            if (current == nullptr)
            {
                return 0;
            }
            else
            {
                current->size--;
                current->sub_sum -= x;
                if (x == current->get_value() && current->same_count)
                {
                    current->same_count--;
                    if (check_balance(root))
                    {
                        remake();
                    }
                    return 0;
                }
                else
                {
                    if (x > current->get_value())
                    {
                        return erase(current->get_right_node(), x);
                    }
                    else
                    {
                        return erase(current->get_left_node(), x);
                    }
                }
            }
        }
        long long queryk(Nodeptr current, const long long x)
        {
            int this_rank = get_node_size(current->get_left_node()) + current->same_count;

            if (this_rank < x)
            {
                long long new_start = x - this_rank;
                return queryk(current->get_right_node(), new_start);
            }
            else if (this_rank == x && current->same_count)
            {
                return current->get_value();
            }
            else
            {
                if (this_rank - x < current->same_count)
                {
                    return current->get_value();
                }
                // go left
                return queryk(current->get_left_node(), x);
            }
        }
        void minimize_ksum(const long long k, Nodeptr current, long long previous_left, long long &delta)
        {
            if (!current)
            {
                return;
            }
            long long current_value = current->get_value() * exist(current->same_count);
            long long left_sum = previous_left;
            if (current->get_left_node())
            {
                left_sum += current->get_left_node()->sub_sum;
            }
            long long right_sum = root->sub_sum - left_sum;
            long long k_sum = left_sum * k;

            if (k_sum < right_sum)
            {
                // test
                right_sum -= current_value;
                k_sum += current_value * k;
                // the first left_sum bigger than right
                if (k_sum >= right_sum)
                {
                    delta = k_sum - right_sum;
                    return;
                }
                else
                {
                    // still smaller, update
                    previous_left += current_value;
                    if (current->get_left_node())
                    {
                        previous_left += current->get_left_node()->sub_sum;
                    }
                    current = current->get_right_node();
                    minimize_ksum(k, current, previous_left, delta);
                }
            }
            else
            {
                // no room for optimization
                if (!current->get_left_node())
                {
                    return;
                }
                else
                {
                    if (k_sum - right_sum <= delta)
                    {
                        delta = k_sum - right_sum;
                    }
                    current = current->get_left_node();
                    minimize_ksum(k, current, previous_left, delta);
                }
            }
        }
        void manual_delete()
        {
        }
        ScapeGoat() = default;
};
ScapeGoat *tree;
// you can do data structure initialization in this function
void init(std::size_t n [[maybe_unused]])
{
    tree = new ScapeGoat();
}

void clear()
{
    delete tree;
}
// opt==1 insert x
// opt==2 erase x
// opt==3 query x
// opt==4 query xth
// opt==5 query x*sum(s1->->->st) - sum(s_t+1->->->s_m)

long long SetOperation(std::size_t opt, long long x)
{
    long long res = 0;
    if (opt == 1)
        res = tree->insert(tree->root, x);
    else if (opt == 2)
        res = tree->erase(tree->root, x);
    else if (opt == 3)
        res = tree->query(tree->root, x);
    else if (opt == 4)
        res = tree->queryk(tree->root, x);
    else
    {
        tree->minimize_ksum(x, tree->root, 0, tree->Delta);
        res = tree->Delta;
    }
    return res;
}
#endif // CS10123F_PA2_BST