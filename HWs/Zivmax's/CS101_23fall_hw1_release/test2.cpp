#include <deque>
#include <queue>
#include <stack>
#include <iostream>
#include <cassert>
#include <vector>

struct car
{
    int id;
    car(int _id) : id(_id) {}

    bool operator==(const car &other) const
    {
        return id == other.id;
    }

    bool operator!=(const car &other) const
    {
        return !(id == other.id);
    }
};

bool is_feasible(const std::deque<car> &enter_seq, std::queue<car>
                                                       exit_seq)
{
    std::stack<car> x; // Choose your data structure
    for (auto &enter_car : enter_seq)
    {
        x.push(enter_car);
        while (!x.empty() && x.top() == exit_seq.front())
        {
            exit_seq.pop();
            x.pop();
        }
    }
    if (x.empty())
    {
        return true; // Given enter_seq, the exit_seq is feasible.
    }
    return false; // Given enter_seq, the exit_seq is not feasible.
}

void test_is_feasible(const std::vector<std::deque<car>> &enter_seqs, const std::vector<std::deque<car>> &exit_seqs)
{
    for (std::size_t i = 0; i < exit_seqs.size(); ++i)
    {
        const bool result = is_feasible(enter_seqs[0], std::queue<car>(exit_seqs[i]));
        std::cout << "Test Case " << i + 1 << ": " << (result ? "Feasible" : "Infeasible") << "\n";
    }
}


int main()
{
    // Test Case 1: Feasible sequences
    std::deque<car> enter_seq1 = {car(1), car(2), car(3)};
    std::queue<car> exit_seq1(std::deque<car>{car(2), car(1), car(3)});
    assert(is_feasible(enter_seq1, exit_seq1));

    // Test Case 2: Infeasible sequences
    std::deque<car> enter_seq2 = {car(1), car(2), car(3)};
    std::queue<car> exit_seq2(std::deque<car>{car(3), car(1), car(2)});
    assert(!is_feasible(enter_seq2, exit_seq2));

    // Test Case 3: Empty sequences
    std::deque<car> enter_seq3;
    std::queue<car> exit_seq3;
    assert(is_feasible(enter_seq3, exit_seq3));

    // Test Case 4: Different order but feasible
    std::deque<car> enter_seq4 = {car(1), car(2), car(3)};
    std::queue<car> exit_seq4(std::deque<car>{car(1), car(2), car(3)});
    assert(is_feasible(enter_seq4, exit_seq4));

    // Test Case 5: Different order and infeasible
    std::deque<car> enter_seq5 = {car(1), car(2), car(3)};
    std::queue<car> exit_seq5(std::deque<car>{car(3), car(1), car(2)});
    assert(!is_feasible(enter_seq5, exit_seq5));

    std::cout << "All test cases passed!\n";

    std::vector<std::deque<car>> enter_seqs = {
        {car(1), car(2), car(3), car(4), car(5), car(6)},
    };

    std::vector<std::deque<car>> exit_seqs = {
        {car(3), car(1), car(5), car(6), car(2), car(4)},
        {car(3), car(5), car(4), car(2), car(6), car(1)},
        {car(2), car(6), car(3), car(4), car(1), car(5)},
        {car(4), car(5), car(6), car(1), car(3), car(2)},
        {car(1), car(4), car(5), car(3), car(2), car(6)}
    };

    test_is_feasible(enter_seqs, exit_seqs);

    return 0;
}
