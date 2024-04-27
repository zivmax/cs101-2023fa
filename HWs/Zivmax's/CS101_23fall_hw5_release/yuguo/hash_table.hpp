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
    string web_string = S.name;
      uint32_t res = 0;
      for (char s : web_string) {
        res = (s + res * base) % 4294967296;
      }
      return res;

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
    std::size_t Start = std::hash<Key>{}(x) % Mod;
    std::size_t pos = Start;
    std::size_t find_pos = this->find(x);
    if(find_pos != npos){
      return find_pos;
    }
    while (State[pos] == entrystate::Used) {
      pos = (pos + 1) % Mod;
    }
    return pos;
  }

  std::size_t find(const Key &x) const {
	  std::size_t Start = std::hash<Key>{}(x) % Mod;
    std::size_t pos = Start;
    while (State[pos] != entrystate::Unused) {
      if (State[pos] == entrystate::Used && Stored_Key[pos] == x){
        return pos;
      }  
      pos = (pos + 1) % Mod;
    }
    return npos;
  }

  void insert(const Key &x, const T& value) {
      std::size_t searched_pos = this->Search(x);
      if(this->State[searched_pos] == entrystate::Used){
        this->Table[searched_pos] = value;
        return;
      }
      // prevent mutiple push_back when erased
      if (this->State[searched_pos] == entrystate::Unused) {
        this->Used_pos.push_back(searched_pos);
      }
      // erased or unused place requires to restablish the key
      this->Stored_Key[searched_pos] = x;
      this->State[searched_pos] = entrystate::Used;
      this->Table[searched_pos] = value;
  }

  bool erase(const Key &x) {
      std::size_t Finded_pos = 0;
      Finded_pos = this->find(x);
      if(Finded_pos == npos){
        return false;
      }
      this->State[Finded_pos] = entrystate::Erased;
      
      return true;
  }

  void clear() {
      for (std::size_t i = 0; i < this->Used_pos.size(); i++) {
          // stored pos could be erased or used
          if (this->State[this->Used_pos[i]] != entrystate::Erased) {
			      this->State[this->Used_pos[i]] = entrystate::Erased;
          }
      }
      this->Used_pos.clear();
  }
};

#endif // CS10123F_PA1_HASHTABLE