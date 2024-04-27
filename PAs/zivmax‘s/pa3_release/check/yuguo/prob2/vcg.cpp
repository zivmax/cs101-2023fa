// @author: yuguo

#include <iostream>
#include <vector>


class Buyer_details
{
    public:
        size_t _v;
        int _c;
        Buyer_details(size_t v, int c) : _v(v), _c(c) { }
};
void print_vector(std::vector<int> buyers)
{
    for (auto i : buyers)
    {
        std::cout << i + 1 << " ";
    }
    std::cout << std::endl;
}
int main()
{
    int n, m, c;
    size_t v;
    std::cin >> n >> m;
    std::vector<Buyer_details> buyer_details;
    std::vector<int> buyers;
    // use MSV to store the transition formula
    // top down
    std::vector<std::vector<size_t>> MSV_td(n + 2, std::vector<size_t>(m + 2, 0));
    // bottom up
    std::vector<std::vector<size_t>> MSV_bu(n + 2, std::vector<size_t>(m + 2, 0));
    buyer_details.push_back(Buyer_details(0, 0));
    for (int i = 0; i < n; ++i)
    {
        std::cin >> v >> c;
        buyer_details.push_back(Buyer_details(v, c));
    }
    // do top dpwn
    for (int i = n; i >= 1; --i)
    {
        for (int j = 0; j <= m; ++j)
        {
            MSV_td[i][j] = MSV_td[i + 1][j];
            if (buyer_details[i]._c <= j)
            {
                size_t includeValue = MSV_td[i + 1][j - buyer_details[i]._c] + buyer_details[i]._v;
                size_t excludeValue = MSV_td[i][j];
                MSV_td[i][j] = std::max(includeValue, excludeValue);
            }
        }
    }
    // do bottom up
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 0; j <= m; ++j)
        {
            MSV_bu[i][j] = MSV_bu[i - 1][j];
            if (buyer_details[i]._c <= j)
            {
                size_t includeValue = MSV_bu[i - 1][j - buyer_details[i]._c] + buyer_details[i]._v;
                size_t excludeValue = MSV_bu[i][j];
                MSV_bu[i][j] = std::max(includeValue, excludeValue);
            }
        }
    }

    // track the buyers
    // using top down
    int i = 1, j = m;
    size_t max_value = 0;
    while (i <= n)
    {
        if (j >= buyer_details[i]._c && MSV_td[i][j] == MSV_td[i + 1][j - buyer_details[i]._c] + buyer_details[i]._v)
        {
            buyers.push_back(i);
            max_value += buyer_details[i]._v;
            std::cout << i << " ";
            j -= buyer_details[i]._c;
        }
        i++;
    }
    std::cout << std::endl;
    // calculate the payment
    size_t p = 0;
    while (p < buyers.size())
    {
        size_t res = buyer_details[buyers[p]]._v;
        size_t MSV_i = 0;
        for (int q = 0; q <= m; ++q)
        {
            MSV_i = std::max(MSV_i, MSV_bu[buyers[p] - 1][q] + MSV_td[buyers[p] + 1][m - q]);
        }
        res += MSV_i - max_value;
        std::cout << res << " ";
        p++;
    }
    return 0;
}