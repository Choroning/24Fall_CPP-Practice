/**
 * @file    Example_NoexceptMove.cpp
 * @brief   Shows that vector reallocation moves elements only when the move constructor is noexcept
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
using namespace std;

template <bool NoThrow>
struct Item {
    string tag;
    explicit Item(string t) : tag(t) {}
    Item(const Item& o) : tag(o.tag) { cout << " copy(" << tag << ")"; }
    Item(Item&& o) noexcept(NoThrow) : tag(std::move(o.tag)) { cout << " move(" << tag << ")"; }
};

template <bool NoThrow>
void grow(const char* label) {
    vector<Item<NoThrow>> v;
    v.reserve(2);
    v.emplace_back("a");
    v.emplace_back("b");
    cout << label << " capacity " << v.capacity() << " ->";
    v.emplace_back("c");                         // exceeds the capacity: reallocation
    cout << "  (capacity " << v.capacity() << ")" << endl;
}

int main() {
    cout << boolalpha;
    cout << "nothrow move: " << is_nothrow_move_constructible<Item<true>>::value
         << ", " << is_nothrow_move_constructible<Item<false>>::value << endl;
    grow<true>("noexcept(true) ");
    grow<false>("noexcept(false)");
    return 0;
}
