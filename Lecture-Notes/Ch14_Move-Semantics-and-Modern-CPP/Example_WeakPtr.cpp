/**
 * @file    Example_WeakPtr.cpp
 * @brief   A reference cycle of shared_ptr leaks; replacing one direction with weak_ptr breaks the cycle
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <memory>
#include <string>
using namespace std;

struct Node {
    string name;
    shared_ptr<Node> next;                       // owning link
    weak_ptr<Node> prev;                         // non-owning link back
    shared_ptr<Node> badPrev;                    // owning link back: creates a cycle
    explicit Node(string n) : name(n) { cout << "  create " << name << endl; }
    ~Node() { cout << "  destroy " << name << endl; }
};

int main() {
    cout << "1. cycle with two shared_ptr links" << endl;
    {
        auto a = make_shared<Node>("A");
        auto b = make_shared<Node>("B");
        a->next = b;
        b->badPrev = a;
        cout << "  use_count A=" << a.use_count() << ", B=" << b.use_count() << endl;
    }                                            // A and B are never destroyed
    cout << "  end of block 1" << endl;

    cout << "2. back link as weak_ptr" << endl;
    weak_ptr<Node> watcher;
    {
        auto c = make_shared<Node>("C");
        auto d = make_shared<Node>("D");
        c->next = d;
        d->prev = c;                             // does not raise C's count
        watcher = d;
        cout << "  use_count C=" << c.use_count() << ", D=" << d.use_count() << endl;
        if (shared_ptr<Node> p = d->prev.lock())  // lock() gives a shared_ptr or null
            cout << "  D's previous node is " << p->name << endl;
    }
    cout << "  end of block 2, watcher expired = " << boolalpha << watcher.expired() << endl;
    return 0;
}
