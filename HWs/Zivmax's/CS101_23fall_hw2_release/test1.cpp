#include <iostream>

int hashFunction(int k)
{
    return (3 * k + 6) % 7;
}

int main()
{
    int key;

    std::cout << "Enter a key: ";
    std::cin >> key;

    int hashValue = hashFunction(key);

    std::cout << "Hash value for key " << key << ": " << hashValue << std::endl;

    return 0;
}
