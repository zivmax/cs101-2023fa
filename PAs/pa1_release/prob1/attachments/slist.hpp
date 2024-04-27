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
  reference operator*() const;

  pointer operator->() const { return std::addressof(operator*()); }

  // TODO: IMPLEMENT THIS.
  self_type &operator++();

  self_type operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  // TODO: IMPLEMENT THIS.
  template <bool C> bool operator==(const SlistIterator<T, C> &other) const;
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
  Slist(const Slist &other);

  void swap(Slist &other) noexcept { m_head.swap(other.m_head); }

  Slist &operator=(const Slist &other) {
    Slist(other).swap(*this);
    return *this;
  }

  Slist(Slist &&) noexcept = default;

  Slist &operator=(Slist &&) noexcept = default;

  ~Slist() = default;

  // TODO: IMPLEMENT THIS.
  Slist(size_type count, const T &value);

  // TODO: IMPLEMENT THIS.
  explicit Slist(size_type count);

  // TODO: IMPLEMENT THIS.
  Slist(std::input_iterator auto b, std::input_iterator auto e);

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
  template <typename... Types> reference emplace_front(Types &&...args);

  // TODO: IMPLEMENT THIS.
  void pop_front();

  void clear() {
    m_head.reset(); // Why?
  }

  // TODO: IMPLEMENT THIS.
  void insert_after(const_iterator pos, const T &x);

  // TODO: IMPLEMENT THIS.
  void insert_after(const_iterator pos, T &&x);

  // TODO: IMPLEMENT THIS.
  template <typename... Types>
  iterator emplace_after(const_iterator pos, Types &&...args);

  // TODO: IMPLEMENT THIS.
  iterator erase_after(const_iterator pos);

  // TODO: IMPLEMENT THIS.
  iterator erase_after(const_iterator pos, const_iterator last);

  // TODO: IMPLEMENT THIS.
  bool empty() const;

  // TODO: IMPLEMENT THIS.
  size_type size() const;

  reference front() { return m_head->value; }
  const_reference front() const { return m_head->value; }

  bool operator==(const Slist<T> &other) const {
    return std::equal(begin(), end(), other.begin(), other.end());
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
  void merge(Slist<T> &&other, auto compare);

  void sort() { sort(std::less<>{}); }

  // TODO: IMPLEMENT THIS.
  void sort(auto compare);
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