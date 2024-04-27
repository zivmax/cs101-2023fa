#ifndef CS10123F_PA1_HASHTABLE
#define CS10123F_PA1_HASHTABLE

#include "hash_types.hpp"
#include <cstdint>
#include <cstdio>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

template <>
struct std::hash<Website>
{
        static const uint32_t base = 1009u; // 1009 is a prime number.
        uint32_t operator()(const Website &S) const noexcept
        {
            std::cerr << "Hash" << std::endl;
            // ------------------------START YOUR CODE----------------
            uint32_t sum = 0;
            for (size_t i = 0; i < S.name.length(); i++)
            {
                sum = S.name.at(i) + sum * base;
            }
            return sum;
            // ------------------------END OF YOUR CODE----------------
        }
};

template <class Key = Website, class T = IPv4> // template for key type class
                                               // Key and value type class T
class HashTable
{
    private:
        enum entrystate { Unused,
                          Used,
                          Erased };
        const size_t Mod = 1000037u;   // 1000037 is a prime number
        std::vector<T> Table;          // main table stores value
        std::vector<Key> Stored_Key;   // used for storing original key
        std::vector<entrystate> State; // keeps the state of every table entry : {Unused, Used or Erased};
        std::vector<size_t> Used_pos;  // hint: used for function clear()

    public:
        static const std::size_t npos = -1;
        HashTable() : Table(Mod), Stored_Key(Mod), State(Mod, entrystate::Unused) { }

        const T &operator[](const Key &x) const
        {
            std::size_t pos = find(x);
            if (pos == npos)
                throw std::invalid_argument("Key does not exist");
            return Table[pos];
        }

        // ------------------------START YOUR CODE----------------

        std::size_t Search(const Key &x) const
        {
            std::cerr << "Search" << std::endl;
            std::size_t Start = std::hash<Key>{}(x) % Mod;
            size_t pos = Start;

            // Check updating
            size_t cpos = find(x);
            if (cpos != npos) { return cpos; }

            // Linear probing
            while (State.at(pos) == entrystate::Used)
            {
                pos = (pos + 1) % Mod;
                if (pos == Start) { return npos; }
            }

            return pos;
        }

        std::size_t find(const Key &x) const
        {
            std::cerr << "Find" << std::endl;
            std::size_t Start = std::hash<Key>{}(x) % Mod;
            size_t pos = Start;
            while (Stored_Key.at(pos) != x)
            {
                if (State.at(pos) == entrystate::Unused) { return npos; }

                pos = (pos + 1) % Mod;
                if (pos == Start) { return npos; }
            }

            return (State.at(pos) != entrystate::Erased) ? pos : npos;
        }

        void insert(const Key &x, const T &value)
        {
            std::cerr << "Insert" << std::endl;
            size_t pos = Search(x);
            if (pos == npos) { return; }
            Stored_Key.at(pos) = x;
            State.at(pos) = entrystate::Used;
            Table.at(pos) = value;
            Used_pos.push_back(pos);
        }

        bool erase(const Key &x)
        {
            std::cerr << "Erase" << std::endl;
            size_t pos = find(x);
            bool result = false;
            if (pos != npos)
            {
                State.at(pos) = entrystate::Erased;
                result = true;
            }

            return result;
        }

        void clear()
        {
            std::cerr << "Clear" << std::endl;
            for (auto &&pos : Used_pos)
            {
                State.at(pos) = entrystate::Erased;
            }
            Used_pos.clear();
        }
};

#endif // CS10123F_PA1_HASHTABLE