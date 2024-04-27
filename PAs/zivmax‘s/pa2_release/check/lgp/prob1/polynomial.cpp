#include <iostream>
#include <vector>
#include <complex>

int get_length(int m, int n)
{
    int len = 1;
    while(len < m + n)
    {
        len *= 2;
    }
    return len;
}

std::vector<std::complex<double>> fft(std::vector<std::complex<double>> poly)
{
    int len = poly.size();
    constexpr auto mag{1.0};
    if(len == 1)
    {
        return poly;
    }
    std::vector<std::complex<double>> polyEven, polyOdd;
    int i = 0;
    for(auto & it : poly)
    {
        if(i == 0)
        {
            polyEven.push_back(it);
            i = 1;
        }else
        {
            polyOdd.push_back(it);
             i = 0;
        }
    }
    std::vector<std::complex<double>> even = fft(polyEven);
    std::vector<std::complex<double>> odd = fft(polyOdd);
    std::vector<std::complex<double>> y;
    y.resize(len);
    std::complex<double> w;
    for(int j = 0; j < len / 2; j++)
    {
        w = std::polar(mag, 2 * j * std::numbers::pi / static_cast<double>(len));
        y[j] = even[j] + w * odd[j];
        y[j + len/2] = even[j] - w * odd[j];
    }
    return y;
}

std::vector<std::complex<double>> ifft(std::vector<std::complex<double>> poly) {
    int len = poly.size();
    constexpr auto mag{1.0};
    if (len == 1) {
        return poly;
    }
    std::vector<std::complex<double>> polyEven, polyOdd;
    int i = 0;
    for(auto & it : poly)
    {
        if(i == 0)
        {
            polyEven.push_back(it);
            i = 1;
        }else
        {
            polyOdd.push_back(it);
            i = 0;
        }
    }
    std::vector<std::complex<double>> even = ifft(polyEven);
    std::vector<std::complex<double>> odd = ifft(polyOdd);
    std::vector<std::complex<double>> y;
    y.resize(len);
    std::complex<double> w;
    for (int j = 0; j < len / 2; j++) {
        w = std::polar(mag, -2 * j * std::numbers::pi / static_cast<double>(len));
        y[j] = even[j] +  w * odd[j];
        y[j + len / 2] = even[j] - w * odd[j];
    }
    return y;
}


int main() {
    int m, n;
    int val;
    std::cin >> m >> n;
    std::vector<std::complex<double>>a, b, c;
    for(int i = 0; i < m; i++){
        std::cin >> val;
        a.emplace_back(val);
    }
    for(int i = 0; i < n; i++){
        std::cin >> val;
        b.emplace_back(val);
    }
    int len = get_length(m, n);
    a.resize(len);
    b.resize(len);
    a = fft(a);
    b = fft(b);
    for(int i = 0; i < len; i++)
    {
        c.push_back(a[i] * b[i]);
    }
    c = ifft(c);
    int outLen = static_cast<int>(c.size());
    for(int i = 0; i < m + n - 1; i++)
    {
        double ret = c[i].real() / outLen;
        if(ret - round(ret) < 1 * std::pow(10, -14) || round(ret) - ret < 1 * std::pow(10, -14))
        {
            std::cout << int(ret + 0.1) << ' ';
        }else
        {
            std::cout << ret << ' ';
        }
    }
    return 0;
}
