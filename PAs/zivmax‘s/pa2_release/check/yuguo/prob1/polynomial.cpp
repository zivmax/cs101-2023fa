#include <complex>
#include <iostream>
#include <numbers>
#include <vector>
using namespace std;
int get_pow(int n)
{
    int res = 1;
    while (res < n)
    {
        res *= 2;
    }
    return res;
}
void print_complex(const vector<complex<double>> &a, int length)
{
    int n = a.size();
    for (int i = 0; i < length; i++)
    {
        if (a[i].real() <= 1e-15)
        {
            cout << 0 << " ";
            continue;
        }
        // +0.5 restrains the value in a certain range a-0.5, a+0.5
        int num = a[i].real() / n + 0.5;
        cout << num << " ";
    }
    cout << endl;
}
void get_even_odd(vector<complex<double>> &even_sub, vector<complex<double>> &odd_sub, const vector<complex<double>> &original)
{
    auto it = original.begin();
    int count = 0;
    while (it != original.end())
    {
        // even index
        if (count % 2 == 0)
        {
            even_sub.push_back(*it);
        }
        // odd index
        else
        {
            odd_sub.push_back(*it);
        }
        it++;
        count++;
    }
}
// calculate the FFT of a array
vector<complex<double>> FFT(const vector<complex<double>> &a, int INV)
{

    int length = a.size();
    vector<complex<double>> result(length);
    if (length <= 1)
    {
        result = a;
        return result;
    }
    vector<complex<double>> even_sub;
    vector<complex<double>> odd_sub;
    get_even_odd(even_sub, odd_sub, a);
    vector<complex<double>> even_sub_fft = FFT(even_sub, INV);
    vector<complex<double>> odd_sub_fft = FFT(odd_sub, INV);

    for (int i = 0; i < length / 2; i++)
    {
        auto ex = polar(1.0, INV * 2 * numbers::pi * i / length);
        result[i] = even_sub_fft[i] + ex * odd_sub_fft[i];
        result[i + length / 2] = even_sub_fft[i] - ex * odd_sub_fft[i];
    }
    return result;
}

vector<complex<double>> multiply(const vector<complex<double>> &a, const vector<complex<double>> &b)
{
    vector<complex<double>> a_fft = FFT(a, 1);
    vector<complex<double>> b_fft = FFT(b, 1);
    int n = a_fft.size();
    for (int i = 0; i < n; i++)
    {
        a_fft[i] *= b_fft[i];
    }
    vector<complex<double>> res = FFT(a_fft, -1);

    return res;
}

int main()
{
    int n, m;
    cin >> n >> m;
    double temp;
    complex<double> complex_temp;
    vector<complex<double>> a;
    vector<complex<double>> b;
    for (int i = 0; i < n; i++)
    {
        cin >> temp;
        a.push_back(temp);
    }
    for (int i = 0; i < m; i++)
    {
        cin >> temp;
        b.push_back(temp);
    }

    int two_pow = get_pow(n + m);
    for (int i = 0; i < two_pow - n; i++)
    {
        a.push_back(0);
    }
    for (int i = 0; i < two_pow - m; i++)
    {
        b.push_back(0);
    }
    auto res = multiply(a, b);
    print_complex(res, m + n - 1);

    return 0;
}