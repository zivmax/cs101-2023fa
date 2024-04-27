#ifndef CS101_SLIST_HPP
#define CS101_SLIST_HPP

#include <algorithm>
#include <concepts>
#include <iterator>
#include <memory>

#include <iostream>

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
        }

        auto base() const { return m_node; }
        auto next_node() const { return base()->next; }
        // TODO: IMPLEMENT THIS.
        reference operator*() const
        {
            return m_node->value;
        }

        pointer operator->() const { return std::addressof(operator*()); }

        // TODO: IMPLEMENT THIS.
        self_type &operator++()
        {
            m_node = m_node->next.get();
            // *this = SlistIterator(m_node->next);
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
        Slist(const Slist &other) : Slist(other.begin(), other.end())
        {
            // if (other.m_head != nullptr) {
            //     m_head = std::make_unique<node_type>(nullptr, other.m_head->value);
            //     iterator m_it(m_head);
            //     iterator other_it(other.m_head);
            //     while (other_it.base()->next != nullptr) {
            //       insert_after(m_it, other_it.base()->value);
            //       other_it++, m_it++;
            //     }
            // }
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
        Slist(size_type count, const T &value) : Slist()
        {
            for (size_type i = 0; i < count; i++)
            {
                this->emplace_front(value);
            }
        }

        // TODO: IMPLEMENT THIS.
        explicit Slist(size_type count) : Slist(count, T{}) { }

        // TODO: IMPLEMENT THIS.
        Slist(std::input_iterator auto b, std::input_iterator auto e)
        {
            if (b == e)
            {
                m_head.reset();
                return;
            }
            emplace_front(*b);
            iterator now_node_it = begin();
            for (auto it = ++b; it != e; it++)
            {
                emplace_after(now_node_it, *it);
                ++now_node_it;
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
            // node_uptr new_head =
            m_head = std::make_unique<node_type>(std::move(m_head), std::forward<Types>(args)...);
            return m_head->value;
        }

        // TODO: IMPLEMENT THIS.
        void pop_front()
        {
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
            auto pos_node = pos.base();
            node_uptr next_node = std::make_unique<node_type>(std::move(pos_node->next), std::forward<Types>(args)...);
            pos_node->next = std::move(next_node);
            return iterator(pos_node->next);
        }

        // TODO: IMPLEMENT THIS.
        iterator erase_after(const_iterator pos)
        {
            auto pos_node = pos.base();
            pos_node->next = std::move(pos_node->next->next);
            return iterator(pos_node->next);
        }

        // TODO: IMPLEMENT THIS.
        iterator erase_after(const_iterator pos, const_iterator last)
        {
            if (pos.base() == last.base())
            {
                return iterator(pos.base()->next);
            }
            iterator it = erase_after(pos);
            while (it.base() != last.base())
            {
                it = erase_after(pos);
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
            // if (this->empty()) return 0;
            size_type sum = 0;
            for (iterator it(m_head); it != end(); it++)
            {
                sum++;
            }
            return sum;
        }

        reference front() { return m_head->value; }
        const_reference front() const { return m_head->value; }

        bool operator==(const Slist<T> &other) const
        {
            return std::equal(begin(), end(), other.begin(), other.end());
        }

        auto operator<=>(const Slist<T> &other) const
        {
            return std::lexicographical_compare_three_way(begin(), end(), other.begin(),
                                                          other.end());
        }

        void merge(Slist<T> &other) { merge(std::move(other)); }

        void merge(Slist<T> &&other) { merge(std::move(other), std::less<>{}); }

        void merge(Slist<T> &other, auto compare)
        {
            merge(std::move(other), compare);
        }

        // TODO: IMPLEMENT THIS.
        void insert_node(node_uptr &a, Slist<T> &other)
        { // insert the head of other before a
            a.swap(other.m_head);
            other.m_head.swap(a->next);
        }

        void merge(Slist<T> &&other, auto compare)
        {
            if (&other == this) return;
            while (!other.empty() && compare(other.front(), front()))
            {
                insert_node(m_head, other);
            }
            iterator it = begin();
            while (!other.empty())
            {
                while (it.base()->next != nullptr && compare(it.base()->next->value, other.front()))
                    it++;
                insert_node(it.base()->next, other);
            }
        }

        // void insert_node(node_uptr &a, node_uptr &b) { // insert b->next before a
        //   b->next = std::move(b->next->next);
        //   a.swap(b->next);

        // }

        void merge_sort(node_uptr &head, iterator sta, auto compare)
        {
            iterator check = sta;
            if (++check ==  this->end()) return;
            // if (sta.base()->next == nullptr) return;
            iterator mid = sta;
            size_type sz =  this->size(), cnt = 0;
            for (; mid !=  this->end(); mid++)
            {
                cnt++;
                if (cnt >= sz / 2) break;
            }
            Slist<T> l_list, r_list;
            node_uptr r_head = std::move(mid.base()->next);
            l_list.m_head = std::move(head);
            r_list.m_head = std::move(r_head);
            std::cout << l_list << "\n";
            std::cout << r_list << "\n\n";
            r_list.merge_sort(r_list.m_head, iterator(r_list.m_head), compare);
            l_list.merge_sort(l_list.m_head, sta, compare);
            l_list.merge(r_list, compare);
            // merge(r_list, compare);
            head = std::move(l_list.m_head);
        }

        void sort() { sort(std::less<>{}); }

        // TODO: IMPLEMENT THIS.
        void sort(auto compare)
        {
            if (empty()) return;
            merge_sort(m_head, iterator(m_head), compare);


            // size_type sz = size();
            // node_uptr fake_head = std::make_unique<node_type>(std::move(m_head));
            // for (size_type len = 2; len / 2 < sz; len <<= 1) {
            //   if (len == 4)
            //     std::cout << "hrer\n";
            //   iterator it2 = begin();
            //   size_type cnt = 0;
            //   node_uptr &cur_node = it2.base()->next;
            //   while (cnt < len / 2 - 1) {
            //     cur_node = it2.base()->next;
            //     cnt++, it2++;
            //   }

            //   cnt = 0;
            //   while (compare(it2.base()->next->value, m_head->value) && cnt < len / 2) {
            //     insert_node(m_head, it2.base()->next);
            //     cnt++;
            //   }
            //   iterator it1 = begin();
            //   for (; it2.base()->next != nullptr && cnt < len / 2; cnt++) {
            //     while (compare(it1.base()->next->value, it2.base()->next->value))
            //       it1++;
            //     if (it1 == it2) {
            //       cur_node = it2.base()->next;
            //       it2++;
            //     }
            //     else {
            //       insert_node(it1.base()->next, it2.base()->next);
            //     }
            //   }
            //   while (it2.base()->next != nullptr) {
            //     T a = it1.base()->next->value, b = it2.base()->next->value;
            //     // std::cout << a << ' ' << b << std::endl;
            //     cnt = 0;
            //     it1 = it2;
            //     while (it2.base()->next != nullptr && cnt < len / 2)
            //       cnt++, it2++;
            //     if (it2.base()->next == nullptr) break;
            //     for (cnt = 0; it2.base()->next != nullptr && cnt < len / 2; cnt++) {
            //       while (compare(it1.base()->next->value, it2.base()->next->value))
            //         it1++;
            //       if (it1 == it2) it2++;
            //       else insert_node(it1.base()->next, it2.base()->next);
            //     }
            //   }
            // }
        }
};

template <std::input_iterator Iterator>
Slist(Iterator, Iterator)
    -> Slist<typename std::iterator_traits<Iterator>::value_type>;

std::ostream &operator<<(std::ostream &ostr, const Slist<int> &list)
{
    for (const int i : list)
        ostr << ' ' << i;
    return ostr;
}

namespace std
{
    template <typename T>
    void swap(Slist<T> &a, Slist<T> &b) noexcept
    {
        a.swap(b);
    }
} // namespace std

#endif // CS101_SLIST_HPP