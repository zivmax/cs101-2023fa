#ifndef CS101_SLIST_HPP
#define CS101_SLIST_HPP

#include <algorithm>
#include <iostream>
#include <concepts>
#include <iterator>
#include <memory>
#include <stack>

#include "slist_node.hpp"

template <typename T, bool IsConst>
struct SlistIterator
{
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
  SlistIterator(Other &&other) : m_node{other.base()}
  {
  }

  auto base() const { return m_node; }

  // TODO: IMPLEMENT THIS.
  reference operator*() const { return m_node->value; }

  pointer operator->() const { return std::addressof(operator*()); }

  // TODO: IMPLEMENT THIS.
  self_type &operator++()
  {
    if (m_node != nullptr)
    {
      m_node = m_node->next.get();
    }
    return *this;
  }

  self_type operator++(int)
  {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  // TODO: IMPLEMENT THIS.
  template <bool C>
  bool operator==(const SlistIterator<T, C> &other) const
  {
    // return m_node == other.m_node;
    return base() == other.base();
  }
};

template <typename T>
class Slist
{
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
  Slist(const Slist &other) : Slist(other.begin(), other.end()) {}

  void swap(Slist &other) noexcept { m_head.swap(other.m_head); }

  Slist &operator=(const Slist &other)
  {
    Slist(other).swap(*this);
    return *this;
  }

  Slist(Slist &&) noexcept = default;

  Slist &operator=(Slist &&) noexcept = default;

  ~Slist() = default;

  // TODO: IMPLEMENT THIS.
  Slist(size_type count, const T &value)
  {
    for (size_type i = 0; i < count; i++)
    {
      emplace_front(value);
    }
  }

  // TODO: IMPLEMENT THIS.
  explicit Slist(size_type count)
  {
    for (size_type i = 0; i < count; i++)
    {
      emplace_front();
    }
  }

  // TODO: IMPLEMENT THIS.
  Slist(std::input_iterator auto b, std::input_iterator auto e)
  {
    if (b == e)
      return;
    emplace_front(*b++);
    auto it = const_iterator(m_head);
    for (; b != e; b++)
    {
      it = emplace_after(it, *b);
    }
  }

  Slist(std::initializer_list<T> init) : Slist(init.begin(), init.end()) {}

  iterator begin() { return iterator(m_head); }
  iterator end() { return {}; }
  const_iterator cbegin() const { return const_iterator(m_head); }
  const_iterator cend() const { return {}; }
  const_iterator begin() const { return cbegin(); }
  const_iterator end() const { return cend(); }

  void push_front(const T &x)
  {
    emplace_front(x); // Why?
  }

  void push_front(T &&x)
  {
    emplace_front(std::move(x)); // Why?
  }

  // TODO: IMPLEMENT THIS.
  template <typename... Types>
  reference emplace_front(Types &&...args)
  {
    m_head = std::make_unique<node_type>(std::move(m_head), std::forward<Types>(args)...);
    return m_head->value;
  }

  // TODO: IMPLEMENT THIS.
  void pop_front()
  {
    if (m_head != nullptr)
    {
      m_head = std::move(m_head->next);
    }
  }

  void clear()
  {
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
    if (pos == end())
    {
      return end();
    }
    if (begin() == end())
    {
      m_head = std::make_unique<node_type>(nullptr, std::forward<Types>(args)...);
      return iterator(m_head);
    }
    pos.base()->next = std::make_unique<node_type>(std::move(pos.base()->next), std::forward<Types>(args)...);
    return iterator(pos.base()->next);
  }

  // TODO: IMPLEMENT THIS.
  iterator erase_after(const_iterator pos)
  {
    if (pos.base()->next != nullptr)
    {
      pos.base()->next = std::move(pos.base()->next->next);
    }
    return iterator(pos.base()->next);
  }

  // TODO: IMPLEMENT THIS.
  iterator erase_after(const_iterator pos, const_iterator last)
  {
    if (pos == last)
    {
      return end();
    }
    if (last.base() == nullptr)
    {
      pos.base()->next = nullptr;
    }
    else
    {
      auto tmp = pos;
      ++tmp;
      while (tmp != last)
      {
        pos.base()->next = std::move(tmp.base()->next);
        tmp = pos;
        ++tmp;
      }
    }
    return iterator(pos.base()->next);
  }

  // TODO: IMPLEMENT THIS.
  bool empty() const { return m_head == nullptr; };

  // TODO: IMPLEMENT THIS.
  size_type size() const
  {
    size_type cnt = 0;
    auto ptr = m_head.get();
    while (ptr != nullptr)
    {
      cnt++;
      ptr = ptr->next.get();
    }
    return cnt;
  }

  reference front() { return m_head->value; }
  const_reference front() const { return m_head->value; }

  bool operator==(const Slist<T> &other) const
  {
    return std::ranges::equal(*this, other);
  }

  auto operator<=>(const Slist<T> &other) const
  {
    return std::lexicographical_compare_three_way(begin(), end(), other.begin(), other.end());
  }

  void merge(Slist<T> &other) { merge(std::move(other)); }

  void merge(Slist<T> &&other) { merge(std::move(other), std::less<>{}); }

  void merge(Slist<T> &other, auto compare)
  {
    merge(std::move(other), compare);
  }

  // TODO: IMPLEMENT THIS.

  void merge(Slist<T> &&other, auto compare)
  {
    if (this == &other || other.empty())
    {
      return;
    }
    if (this->empty())
    {
      m_head.swap(other.m_head);
      return;
    }
    auto pos = this->begin();
    if (compare(other.m_head->value, *(pos)) && !other.empty())
    {
      m_head.swap(other.m_head->next);
      m_head.swap(other.m_head);
      pos = this->begin();
    }
    while (1)
    {
      if (other.empty())
      {
        break;
      }
      if (pos.base() != nullptr)
      {
        if (pos.base()->next == nullptr || compare(other.m_head->value, pos.base()->next->value))
        {
          if (pos.base()->next == nullptr || !compare(other.m_head->value, *(pos)))
          {
            other.m_head->next.swap(pos.base()->next);
            pos.base()->next.swap(other.m_head);
          }
          else
          {
            pos++;
          }
        }
        else
        {
          pos++;
        }
      }
      else
      {
        break;
      }
    }
  }

  void sort() { sort(std::less<>{}); }

  // TODO: IMPLEMENT THIS.
  void sort(auto compare)
  {
    if (this->size() <= 1)
    {
      return;
    }
    auto mid = this->begin();
    auto fast = this->begin();
    ++fast;
    while (fast != this->end())
    {
      ++fast;
      if (fast == end())
        break;
      ++mid;
      ++fast;
    }

    Slist<T> other;
    other.m_head.swap(mid.base()->next);
    other.sort(compare);
    sort(compare);
    merge(other, compare);
  }
};

template <std::input_iterator Iterator>
Slist(Iterator, Iterator)
    -> Slist<typename std::iterator_traits<Iterator>::value_type>;

// std::ostream &operator<<(std::ostream &ostr, const Slist<int> &list) {
//   for (const int i : list)
//     ostr << ' ' << i;
//   return ostr;
// }

namespace std
{
  template <typename T>
  void swap(Slist<T> &a, Slist<T> &b) noexcept
  {
    a.swap(b);
  }
} // namespace std

#endif // CS101_SLIST_HPP
