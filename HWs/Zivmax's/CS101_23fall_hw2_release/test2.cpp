#include <iostream>

int quadraticProbing(int h_k, int i) {
    return (h_k + i * i) % 7;
}

int main() {
    int h_k, i;

    // Get input from the user
    std::cout << "Enter the value of h(k): ";
    std::cin >> h_k;

    std::cout << "Enter the value of i: ";
    std::cin >> i;

    int result = quadraticProbing(h_k, i);

    std::cout << "Hi(k) = (" << h_k << " + " << i << "^2) mod 7 = " << result << std::endl;

    return 0;
}
