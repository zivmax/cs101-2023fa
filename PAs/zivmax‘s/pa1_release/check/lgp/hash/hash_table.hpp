#ifndef CS10123F_PA1_HASHTABLE
#define CS10123F_PA1_HASHTABLE

#include <cstdint>
#include <cstdio>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "hash_types.hpp"

template <> struct std::hash<Website> {
  static const uint32_t base = 1009u; // 1009 is a prime number.
  uint32_t operator()(const Website &S) const noexcept {
    // ------------------------START YOUR CODE----------------
    uint32_t ret = 0;
    size_t exp = S.name.size();
    for(auto ch : S.name)
    {
        exp--;
        ret += ch * pow(base, exp);
    }
    return ret;
    // ------------------------END OF YOUR CODE----------------
  }
};

template <class Key = Website, class T = IPv4> // template for key type class
                                               // Key and value type class T
class HashTable {
private:
  enum entrystate { Unused, Used, Erased };
  const size_t Mod = 1000037u; // 1000037 is a prime number
  std::vector<T> Table;        // main table stores value
  std::vector<Key> Stored_Key; // used for storing original key
  std::vector<entrystate>
      State; // keeps the state of every table entry : {Unused, Used or Erased};
  std::vector<size_t> Used_pos; // hint: used for function clear()

public:
  static const std::size_t npos = -1;
  HashTable() : Table(Mod), Stored_Key(Mod), State(Mod, entrystate::Unused) {}

  const T &operator[](const Key &x) const {
    std::size_t pos = find(x);
    if (pos == npos)
      throw std::invalid_argument("Key does not exist");
    return Table[pos];
  }

  // ------------------------START YOUR CODE----------------

  std::size_t Search(const Key &x) const {
      std::cout << 'Search used';
      std::size_t Start = std::hash<Key>{}(x) % Mod;
    std::size_t OriPos = Start;
    std::size_t index = npos;
    while(State.at(Start) != Unused && Stored_Key.at(Start) != x)
    {
        if(index == npos && State.at(Start) == Erased)
        {
            index = Start;
        }
        Start = ( Start +  1 ) % Mod;
        if(Start == OriPos)
        {
            break;
        }
    }
    if(Stored_Key.at(Start) == x || index == npos)
    {
        return Start;
    }
    return index;
  }

  std::size_t find(const Key &x) const {
      std::cout << 'find used';
      size_t pos = Search(x);
      if(State.at(pos) == Unused || State.at(pos) == Erased)
      {
          return npos;
      }
      else
      {
          return pos;
      }
  }

  void insert(const Key &x, const T& value) {
      std::cout << 'insert used';
      std::size_t Start = std::hash<Key>{}(x) % Mod;
      size_t pos = Search(x);
      if(State.at(pos) != Used)
      {
          Table.at(pos) = value;
          Stored_Key.at(pos) = x;
          State.at(pos) = Used;
          Used_pos.push_back(Start);
          return;
      }
      while(State.at(Start) == Used && Start != pos)
      {
          Start = ( Start +  1 ) % Mod;
      }
      if(Start == pos)
      {
          return;
      }
      else
      {
          State.at(pos) = Unused;
          Table.at(Start) = value;
          Stored_Key.at(Start) = x;
          State.at(Start) = Used;
          Used_pos.push_back(Start);
      }
  }

  bool erase(const Key &x) {
      std::cout << 'erase used';
      size_t pos = find(x);
      if(pos == npos)
      {
          return false;
      }
      else
      {
          State.at(pos) = Erased;
          return true;
      }
  }

  void clear() {
      std::cout << 'clear used';
      for(auto pos : Used_pos)
      {
          State.at(pos) = Unused;
      }
  }
};

#endif // CS10123F_PA1_HASHTABLE