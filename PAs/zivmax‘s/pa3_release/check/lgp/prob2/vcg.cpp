#include <iostream>
#include <vector>
using namespace std;

void knapsack(int n, int capacity, vector<int>& wt, vector<int>& val)
{
    vector<vector<size_t>> dp_forward(n + 1, vector<size_t>(capacity + 1, 0));
    vector<vector<size_t>> dp_backward(n + 1, vector<size_t>(capacity + 1, 0));
    vector<vector<bool>> chosen(n + 1, vector<bool>(capacity + 1, false));

    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= capacity; j++)
        {
            if(wt[i - 1] <= j)
            {
                dp_forward[i][j] = std::max(val[i - 1] + dp_forward[i - 1][j - wt[i - 1]], dp_forward[i - 1][j]);
            }
            else
            {
                dp_forward[i][j] = dp_forward[i - 1][j];
            }
        }
    }

    for(int i = n - 1; i >= 0; i--)
    {
        for(int j = 0; j <= capacity; j++)
        {
            if(wt[i] <= j && val[i] + dp_backward[i + 1][j - wt[i]] >= dp_backward[i + 1][j])
            {
                dp_backward[i][j] = val[i] + dp_backward[i + 1][j - wt[i]];
                chosen[i][j] = true;
            }
            else
            {
                dp_backward[i][j] = dp_backward[i + 1][j];
            }
        }
    }

    vector<int> selected_items;
    int a = 0, b = capacity;
    while (a < n && b > 0) {
        if (chosen[a][b]) {
            selected_items.push_back(a);
            b -= wt[a];
        }
        ++a;
    }

    vector<size_t> payment(selected_items.size(), 0);

    int index = 0;
    for(auto& it: selected_items)
    {
        std::cout << it + 1 << " ";
        for(int j = 0; j <= capacity; j++)
        {
            if(val[it] + dp_backward[it + 1][j] + dp_forward[it][capacity - j]  > dp_forward[n][capacity])
                payment[index] = std::max(val[it] + dp_backward[it + 1][j] + dp_forward[it][capacity - j] - dp_forward[n][capacity], payment[index]);
        }
        index++;
    }

    std::cout << std::endl;

    for(unsigned int i = 0; i < selected_items.size(); i++)
    {
        std::cout << payment[i] << " ";
    }
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<int> weights(n);
    std::vector<int> values(n);
    for (int i = 0; i < n; i++) {
        std::cin >> values[i] >> weights[i];
    }

    knapsack(n, m, weights, values);

    return 0;
}

