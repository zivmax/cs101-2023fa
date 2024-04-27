// Include libs
#include <iostream>
#include <string>
#include <cassert>

// Namespace declaration
using namespace std;

struct Node
{
    string word;
    Node *next;
    Node(const string &_word, Node *_next)
        : word(_word), next(_next) {}
};

Node *head;

Node *move_to_first(Node *head, Node *p)
{
    Node *to_move = p->next;
    p->next = to_move->next;
    to_move->next = head;

    return to_move;
}

Node *reverse_list(Node *head)
{
    Node *p = head;
    while (p->next != nullptr)
    {
        head = move_to_first(head, p);
    }
    return head;
}


#include <iostream>
#include <cassert>

// Test function for reverse_list
void test_reverse_list()
{
    // Test case 1
    Node *test_head_1 = new Node("A", new Node("B", new Node("C", nullptr)));
    Node *reversed_1 = reverse_list(test_head_1);
    assert(reversed_1->word == "C");
    assert(reversed_1->next->word == "B");
    assert(reversed_1->next->next->word == "A");
    assert(reversed_1->next->next->next == nullptr);
    std::cout << "Test case 1 passed!\n";

    std::cout << "All tests passed!\n";
}

int main()
{
    // Run the test suite
    test_reverse_list();

    return 0;
}
