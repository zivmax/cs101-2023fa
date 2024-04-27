#ifndef CS101_SLIST_HPP
#define CS101_SLIST_HPP

#include <algorithm>
#include <concepts>
#include <iostream>
#include <iterator>
#include <memory>

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
            : m_node{up.get()} { }

        template <typename Other>
            requires isAddingLowLevelConst<std::decay_t<Other>>
        SlistIterator(Other &&other) : m_node{other.base()}
        {
            return;
        }

        auto base() const { return m_node; }

        // TODO: IMPLEMENT THIS.
        reference operator*() const
        {
            return m_node->value;
        }

        pointer operator->() const { return std::addressof(operator*()); }

        // TODO: IMPLEMENT THIS.
        self_type &operator++()
        {
            m_node = (m_node == nullptr) ? nullptr : m_node->next.get();
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
            return m_node == other.base();
        }
};
Slist<int>

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

        node_uptr m_head = nullptr;

    public:
        Slist() = default;

        // TODO: IMPLEMENT THIS.
        Slist(const Slist &other)
        {
            if (this == &other || other.empty()) { return; }

            const_iterator othersItr = other.begin();
            emplace_front(*(othersItr++));
            iterator itr = begin();

            while (othersItr != end())
            {
                emplace_after(itr, *(othersItr++));
                itr++;
            }
        }

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
            for (size_t i = 0; i < count; i++)
            {
                emplace_front(value);
            }
        }

        // TODO: IMPLEMENT THIS.
        explicit Slist(size_type count)
        {
            for (size_t i = 0; i < count; i++)
            {
                emplace_front();
            }
        }

        // TODO: IMPLEMENT THIS.
        Slist(std::input_iterator auto b, std::input_iterator auto e)
        {
            if (b == e) { return; }

            emplace_front(*(b++));
            iterator itr = begin();
            while (b != e)
            {
                emplace_after(itr, *(b++));
                itr++;
            }
        }

        Slist(std::initializer_list<T> init) : Slist(init.begin(), init.end()) { }

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
            node_uptr tmp = std::make_unique<node_type>(std::move(m_head), std::forward<Types>(args)...);
            m_head = std::move(tmp);

            return m_head->value;
        }

        // TODO: IMPLEMENT THIS.
        void pop_front()
        {
            if (m_head == nullptr) { return; }
            m_head = std::move(m_head->next);
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
            if (pos == end()) { return end(); }

            node_uptr tmp = std::make_unique<node_type>(std::move(pos.base()->next), std::forward<Types>(args)...);
            pos.base()->next = std::move(tmp);

            return iterator(pos.base()->next);
        }

        // TODO: IMPLEMENT THIS.
        iterator erase_after(const_iterator pos)
        {
            if (pos == end()) { return end(); }

            pos.base()->next = std::move(pos.base()->next->next);

            return iterator(pos.base()->next);
        }

        // TODO: IMPLEMENT THIS.
        iterator erase_after(const_iterator pos, const_iterator last)
        {
            if (pos == end()) { return end(); }

            while (iterator(pos.base()->next) != last)
            {
                pos.base()->next = std::move(pos.base()->next->next);
            }

            return iterator(pos.base()->next);
        }


        // TODO: IMPLEMENT THIS.
        bool empty() const
        {
            return m_head == nullptr;
        }

        // TODO: IMPLEMENT THIS.
        size_type size() const
        {
            const_iterator itr = begin();
            size_type size = 0;
            while (itr != end())
            {
                size++;
                itr++;
            }

            return size;
        }

        reference front() { return m_head->value; }
        const_reference front() const { return m_head->value; }

        bool operator==(const Slist<T> &other) const
        {
            return std::ranges::equal(*this, other);
        }

        auto operator<=>(const Slist<T> &other) const
        {
            return std::lexicographical_compare_three_way(begin(), end(), other.begin(),
                                                          other.end());
        }

        void merge(Slist<T> &other) { merge(std::move(other)); }

        void merge(Slist<T> &&other) { merge(std::move(other), std::less<>{}); }

        void merge(Slist<T> &other, auto compare) { merge(std::move(other), compare); }

        // TODO: IMPLEMENT THIS.
        void merge(Slist<T> &&other, auto compare)
        {
            if (this == &other || other.empty()) { return; }
            if (empty()) { other.swap(*this); }
            iterator itr1 = begin();
            if (!other.empty() && (compare(other.front(), *itr1)))
            {
                m_head.swap(other.m_head->next);
                m_head.swap(other.m_head);
                itr1 = begin();
            }
            while (!other.empty())
            {
                if (itr1.base()->next == nullptr)
                {
                    itr1.base()->next.swap(other.m_head);
                }
                else if (!compare(other.front(), *itr1) && compare(other.front(), itr1.base()->next->value))
                {
                    itr1.base()->next.swap(other.m_head);
                    itr1.base()->next->next.swap(other.m_head);
                }
                else { itr1++; }
            }
        }

        void sort() { sort(std::less<>{}); }

        // TODO: IMPLEMENT THIS.
        void sort(auto compare)
        {
            if (m_head == nullptr || m_head->next == nullptr) { return; }

            iterator mid_pos = begin();
            iterator end_pos = begin();
            while (++end_pos != end() && ++end_pos != end())
            {
                ++mid_pos;
            }

            Slist<T> right;

            right.m_head.swap(mid_pos.base()->next);

            this->sort(compare);
            right.sort(compare);
            this->merge(right, compare);
        }
};


template <std::input_iterator Iterator>
Slist(Iterator, Iterator)
    -> Slist<typename std::iterator_traits<Iterator>::value_type>;

namespace std
{
    template <typename T>
    void swap(Slist<T> &a, Slist<T> &b) noexcept
    {
        a.swap(b);
    }
} // namespace std


#endif // CS101_SLIST_HPP