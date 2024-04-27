#include "../hash_table.hpp"

#include <iostream>

int main()
{
    std::size_t hash_value1 = std::hash<Website>{}(Website(std::string("abcde")));
    
    std::cerr << "hash on string abcde\n";
    std::cerr << "Your hash_value: " << hash_value1 << '\n';
    std::cerr << "Expected  value: " << 522585007 << '\n';

    std::size_t hash_value2 = std::hash<Website>{}(Website(std::string("acm.shanghaitech.edu.cn")));
    std::cerr << "hash on string acm.shanghaitech.edu.cn\n";
    std::cerr << "Your hash_value: " << hash_value2 << '\n';
    std::cerr << "Expected  value: " << 3929495537 << '\n';
}