#ifndef CS101_SLIST_HPP
#define CS101_SLIST_HPP

#include <algorithm>
#include <concepts>
#include <iterator>
#include <memory>

#include "slist_node.hpp"

template <typename T, bool IsConst> struct SlistIterator {
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using reference = std::conditional_t<IsConst, const T &, T &>;
  using pointer = std::conditional_t<IsConst, const T *, T *>;
  using iterator_category = std::forward_iterator_tag;

private:
  using node_type = SlistNode<T>;
  using self_type = SlistIterator<T, IsConst>;

  node_type *m_node{};

  template <typename OtherDecayed>
  static constexpr auto isAddingLowLevelConst =
      IsConst && std::is_same_v<OtherDecayed, SlistIterator<T, false>>;

public:
  SlistIterator() = default;

  explicit SlistIterator(const std::unique_ptr<node_type> &up)
      : m_node{up.get()} {}

  template <typename Other>
    requires isAddingLowLevelConst<std::decay_t<Other>>
  SlistIterator(Other &&other) : m_node{other.base()} {}

  auto base() const { return m_node; }

  // TODO: IMPLEMENT THIS.
  reference operator*() const {
      return m_node->value;
  }

  pointer operator->() const { return std::addressof(operator*()); }

  // TODO: IMPLEMENT THIS.
  self_type &operator++(){
      this->m_node = this->m_node->next.get();
      return *this;
  }

  self_type operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  // TODO: IMPLEMENT THIS.
  template <bool C> bool operator==(const SlistIterator<T, C> &other) const
  {
      return this->m_node == other.base();
  }
};

template <typename T> class Slist {
public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using iterator = SlistIterator<T, false>;
  using const_iterator = SlistIterator<T, true>;

private:
  using node_type = SlistNode<T>;
  using node_uptr = std::unique_ptr<node_type>;

  node_uptr m_head;

public:
  Slist() = default;

  // TODO: IMPLEMENT THIS.
  Slist(const Slist &other) {
      if(other.m_head.get() == nullptr)
      {
          return;
      }
      auto otherPtr = other.m_head.get();
      this->m_head = std::make_unique<node_type>(nullptr, otherPtr->value);
      auto thisPtr = this->m_head.get();
      while(otherPtr->next != nullptr){
        thisPtr->next = std::make_unique<node_type>(nullptr, otherPtr->next->value);
        thisPtr = thisPtr->next.get();
        otherPtr = otherPtr->next.get();
      };
  }

  void swap(Slist &other) noexcept { m_head.swap(other.m_head); }

  Slist &operator=(const Slist &other) {
    Slist(other).swap(*this);
    return *this;
  }

  Slist(Slist &&) noexcept = default;

  Slist &operator=(Slist &&) noexcept = default;

  ~Slist() = default;

  // TODO: IMPLEMENT THIS.
  Slist(size_type count, const T &value)
  {
      if(count == 0)
      {
          return;
      }
      this->m_head = std::make_unique<node_type>(nullptr, value);
      auto thisPtr = this->m_head.get();
      for(size_t i = 1; i < count; i++)
      {
          thisPtr->next = std::make_unique<node_type>(nullptr, value);
          thisPtr = thisPtr->next.get();
      }
  }

  // TODO: IMPLEMENT THIS.
  explicit Slist(size_type count)
  {
      T value = T();
      this->m_head = std::make_unique<node_type>(nullptr, value);
      auto thisPtr = this->m_head.get();
      for(size_t i = 1; i < count; i++)
      {
          thisPtr->next = std::make_unique<node_type>(nullptr, value);
          thisPtr = thisPtr->next.get();
      }
  }

  // TODO: IMPLEMENT THIS.
  Slist(std::input_iterator auto b, std::input_iterator auto e)
  {
      if(b == e)
      {
          return;
      }
      this->m_head = std::make_unique<node_type>(nullptr, *b);
      auto thisPtr = this->m_head.get();
      auto it = b;
      it++;
      for(; it != e; it++)
      {
          thisPtr->next = std::make_unique<node_type>(nullptr, *it);
          thisPtr = thisPtr->next.get();
      }
  }

  Slist(std::initializer_list<T> init) : Slist(init.begin(), init.end()) {}

  iterator begin() { return iterator(m_head); }
  iterator end() { return {}; }
  const_iterator cbegin() const { return const_iterator(m_head); }
  const_iterator cend() const { return {}; }
  const_iterator begin() const { return cbegin(); }
  const_iterator end() const { return cend(); }

  void push_front(const T &x) {
    emplace_front(x); // Why?
  }

  void push_front(T &&x) {
    emplace_front(std::move(x)); // Why?
  }

  // TODO: IMPLEMENT THIS.
  template <typename... Types> reference emplace_front(Types &&...args){
      auto ptr = std::make_unique<SlistNode<T>>(std::move(m_head), std::forward<Types>(args)...);
      m_head.swap(ptr);
      return m_head->value;
  }

  // TODO: IMPLEMENT THIS.
  void pop_front(){
      m_head = std::move(m_head->next);
  }

  void clear() {
    m_head.reset(); // Why?
  }

  // TODO: IMPLEMENT THIS.
  void insert_after(const_iterator pos, const T &x)
  {
      emplace_after(pos, x);
  }

  // TODO: IMPLEMENT THIS.
  void insert_after(const_iterator pos, T &&x)
  {
      emplace_after(pos, std::move(x));
  }

  // TODO: IMPLEMENT THIS.
  template <typename... Types>
  iterator emplace_after(const_iterator pos, Types &&...args)
  {
      auto ptr = new SlistNode<T>(nullptr, std::forward<Types>(args)...);
      ptr->next.reset(ptr);
      ptr->next.swap(pos.base()->next);
      auto it = iterator(pos.base()->next);
      return it;
  }

  // TODO: IMPLEMENT THIS.
  iterator erase_after(const_iterator pos)  {
      pos.base()->next = std::move(pos.base()->next->next);
      if(pos.base()->next == nullptr)
      {
          return end();
      }
      iterator it = iterator (pos.base()->next);
      return it;
  }

  // TODO: IMPLEMENT THIS.
  iterator erase_after(const_iterator pos, const_iterator last) {

    auto it = pos;
    for(; it.base()->next.get() != last.base();)
    {
        pos.base()->next = std::move(pos.base()->next->next);
    }
      iterator ret = iterator (it.base()->next);

    return ret;
  }

  // TODO: IMPLEMENT THIS.
  bool empty() const{ return m_head == nullptr; }

  // TODO: IMPLEMENT THIS.
  size_type size() const
  {
      if(this->empty())
      {
          return 0;
      }
      auto it = iterator(m_head);
      size_type count = 1;
      while(it.base()->next.get() != nullptr){
          it++;
          count++;
      }
      return count;
  }

  reference front() { return m_head->value; }
  const_reference front() const { return m_head->value; }

  bool operator==(const Slist<T> &other) const {
    return std::ranges::equal(*this, other);
  }

  auto operator<=>(const Slist<T> &other) const {
    return std::lexicographical_compare_three_way(begin(), end(), other.begin(),
                                                  other.end());
  }

  void merge(Slist<T> &other) { merge(std::move(other)); }

  void merge(Slist<T> &&other) { merge(std::move(other), std::less<>{}); }

  void merge(Slist<T> &other, auto compare) {
    merge(std::move(other), compare);
  }

  // TODO: IMPLEMENT THIS.
  void merge(Slist<T> &&other, auto compare)
  {
      if(this == &other) { return; }
      if(other.m_head == nullptr && this->m_head == nullptr) { return; }
      if(this->m_head == nullptr) {
          other.m_head.swap(this->m_head);
          return;
      }
      if(other.m_head == nullptr) { return; }
      if(compare(other.m_head->value, this->m_head->value))
      {
          this->m_head.swap(other.m_head);
          auto ot = this->begin();
          while (ot.base()->next != nullptr && compare(ot.base()->next->value, other.m_head->value))
              ot++;
          if(ot.base()->next == nullptr)
          {
              ot.base()->next = std::move(other.m_head);
              return;
          }
          else{
              ot.base()->next.swap(other.m_head);
          }
          while(ot.base()->next != nullptr && other.m_head != nullptr)
          {
              if(!compare(other.m_head->value, ot.base()->value) && compare(other.m_head->value, ot.base()->next->value))
              {
                  ot.base()->next.swap(other.m_head);
                  other.m_head.swap(ot.base()->next->next);
              }
              else
                  ot++;
          }
          if(other.m_head != nullptr)
              ot.base()->next = std::move(other.m_head);
          return;
      }
      auto it = this->begin();
      while(it.base()->next != nullptr && other.m_head != nullptr)
      {
          if(!compare(other.m_head->value, it.base()->value) && compare(other.m_head->value, it.base()->next->value))
          {
              it.base()->next.swap(other.m_head);
              other.m_head.swap(it.base()->next->next);
          }
          else
              it++;
      }
      if(other.m_head != nullptr)
          it.base()->next = std::move(other.m_head);
  }

  void sort() { sort(std::less<>{}); }

  // TODO: IMPLEMENT THIS.
  void sort(auto compare)
  {
      Slist other;
      auto it = this->begin();
      if(size() == 1 || empty())
      {
          return;
      }
      for(size_t i = 0; i < size() / 2 - 1; i++)
      {
          it++;
      }
      other.m_head.swap(it.base()->next);
      other.sort(compare);
      this->sort(compare);
      merge(other, compare);
  }
};

template <std::input_iterator Iterator>
Slist(Iterator, Iterator)
    -> Slist<typename std::iterator_traits<Iterator>::value_type>;

namespace std {
template <typename T> void swap(Slist<T> &a, Slist<T> &b) noexcept {
  a.swap(b);
}
} // namespace std

#endif // CS101_SLIST_HPP