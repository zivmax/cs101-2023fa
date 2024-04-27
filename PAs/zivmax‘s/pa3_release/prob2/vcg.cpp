// @author: zivmax

#include <iostream>
#include <vector>

using namespace std;

struct Buyer
{
        size_t v;
        int c;
};

int main()
{
    int n, m;
    cin >> n >> m;
    vector<Buyer> buyers(n + 1);

    for (int i = 1; i <= n; ++i)
    {
        cin >> buyers[i].v >> buyers[i].c;
    }

    // Forward dynamic programming table
    vector<vector<size_t>> dp_forward(n + 1, vector<size_t>(m + 1, 0));

    // Fill the forward dp table
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            if (j >= buyers[i].c && dp_forward[i - 1][j - buyers[i].c] + buyers[i].v > dp_forward[i - 1][j])
            {
                // Taking i-th buyer
                dp_forward[i][j] = dp_forward[i - 1][j - buyers[i].c] + buyers[i].v;
            }
            else
            {
                dp_forward[i][j] = dp_forward[i - 1][j]; // Not taking i-th buyer
            }
        }
    }

    // Backward dynamic programming table
    vector<vector<size_t>> dp_backward(n + 2, vector<size_t>(m + 2, 0));

    // Fill the backward dp table
    for (int i = n; i >= 1; --i)
    {
        for (int j = 1; j <= m; ++j)
        {
            if (j >= buyers[i].c && dp_backward[i + 1][j - buyers[i].c] + buyers[i].v > dp_backward[i + 1][j])
            {
                // Taking i-th buyer
                dp_backward[i][j] = dp_backward[i + 1][j - buyers[i].c] + buyers[i].v;
            }
            else
            {
                dp_backward[i][j] = dp_backward[i + 1][j]; // Not taking i-th buyer
            }
        }
    }

    // Print the backward dp table
    // for (int i = 1; i <= n; ++i)
    // {
    //     for (int j = 1; j <= m; ++j)
    //     {
    //         cout << dp_backward[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;


    // Find the winners
    // Reconstruct the solution to find AM(V,C)
    vector<int> winners;
    int remaining_items = m;
    for (int i = 1; i <= n; ++i)
    {
        if (remaining_items >= buyers[i].c)
        {
            if (buyers[i].v + dp_backward[i + 1][remaining_items - buyers[i].c] == dp_backward[i][remaining_items])
            {
                winners.push_back(i);
                remaining_items -= buyers[i].c;
            }
        }
    }

    // Print the winners (lexicographically least)
    for (int winner : winners)
    {
        cout << winner << " ";
    }
    cout << endl;

    // Compute and print the payments
    for (int winner : winners)
    {
        size_t payment = buyers[winner].v;
        size_t max_val_without_winner = 0;
        for (int k = m; k >= 0; --k)
        {
            max_val_without_winner = max(max_val_without_winner, dp_forward[winner - 1][k] + dp_backward[winner + 1][m - k]);
        }
        payment += max_val_without_winner - dp_forward[n][m];
        cout << payment << " ";
    }
    cout << endl;

    return 0;
}
