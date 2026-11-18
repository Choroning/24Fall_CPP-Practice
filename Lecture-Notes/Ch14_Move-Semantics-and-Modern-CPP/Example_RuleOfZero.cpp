/**
 * @file    Example_RuleOfZero.cpp
 * @brief   A class built only from members that manage themselves, so the compiler writes all five functions
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
using namespace std;

class Playlist {                                 // declares none of the five special functions
    string title;
    vector<string> songs;
public:
    explicit Playlist(string t) : title(std::move(t)) {}
    void add(string s) { songs.push_back(std::move(s)); }
    size_t count() const { return songs.size(); }
    const string& name() const { return title; }
};

class Owner {                                    // unique_ptr makes the class move-only
    unique_ptr<Playlist> list;
public:
    explicit Owner(string t) : list(make_unique<Playlist>(std::move(t))) {}
    bool empty() const { return list == nullptr; }
};

int main() {
    Playlist p("road trip");
    p.add("song A");
    p.add("song B");

    Playlist copy = p;                           // memberwise copy
    Playlist moved = std::move(p);               // memberwise move
    cout << "copy: " << copy.name() << ", " << copy.count() << " songs" << endl;
    cout << "moved: " << moved.name() << ", " << moved.count() << " songs" << endl;
    cout << "source after move: \"" << p.name() << "\", " << p.count() << " songs" << endl;

    cout << boolalpha;
    cout << "Playlist copyable " << is_copy_constructible<Playlist>::value
         << ", nothrow movable " << is_nothrow_move_constructible<Playlist>::value << endl;
    cout << "Owner copyable " << is_copy_constructible<Owner>::value
         << ", movable " << is_move_constructible<Owner>::value << endl;

    Owner o1("x");
    Owner o2 = std::move(o1);                    // ownership moves; o1 keeps a null pointer
    cout << "o1 empty " << o1.empty() << ", o2 empty " << o2.empty() << endl;
    return 0;
}
