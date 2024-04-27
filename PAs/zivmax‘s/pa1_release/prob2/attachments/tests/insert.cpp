#include "../hash_table.hpp"

#include <iostream>

int main()
{
    HashTable<Website, IPv4> table;
    Website acm(std::string("acm.shanghaitech.edu.cn"));
    IPv4 ip(168760789);
    IPv4 ip2(168760780);
    table.insert(acm, ip);
    std::cerr << "from hash table find ip of " << acm << " is " << table[acm] << '\n';
    std::cerr << "expected ip : " << ip << '\n';

    std::cerr << table.Search(acm) << '\n';
    std::cerr << table.find(acm) << '\n';
    table.insert(acm, ip2);
    std::cerr << "from hash table find ip of " << acm << " is " << table[acm] << '\n';
    std::cerr << "expected ip : " << ip2 << '\n';
}