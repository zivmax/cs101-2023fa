#include "../slist.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

template <typename T>
std::ostream &operator<<(std::ostream &s, const Slist<T> &v)
{
    s.put('{');
    for (char comma[]{'\0', ' ', '\0'}; const auto &e : v)
        s << comma << e, comma[0] = ',';
    return s << "}\n";
}

void test_slist_range_ctor()
{

    std::vector<int> data = {1, 2, 3, 4, 5};

    auto b = std::begin(data);
    auto e = std::end(data);

    Slist<int> list(b, e);

    // 检查list大小
    assert(list.size() == data.size());

    // 检查元素顺序
    auto it = list.begin();
    for (size_t i = 0; i < data.size(); ++i)
    {
        assert(*it == data[i]);
        ++it;
    }
    // 更多边界值测试
    Slist<int> empty_list(b, b);
    assert(empty_list.empty());

    Slist<int> one_elm_list(b, std::next(b));
    assert(one_elm_list.size() == 1);
}

int main()
{
    // C++11 initializer list syntax:
    Slist<std::string> words1{"the", "frogurt", "is", "also", "cursed"};
    std::cout << "1: " << words1;

    // words2 == words1
    Slist<std::string> words2(words1.begin(), words1.end());
    std::cout << "2: " << words2;

    // words3 == words1
    Slist<std::string> words3(words1);
    std::cout << "3: " << words3;

    // words4 is {"Mo", "Mo", "Mo", "Mo", "Mo"}
    Slist<std::string> words4(5, "Mo");
    std::cout << "4: " << words4;

    auto const rg = {"cat", "cow", "crow"};
    Slist<std::string> words5(rg.begin(), rg.end());
    std::cout << "5: " << words5;

    test_slist_range_ctor();
}