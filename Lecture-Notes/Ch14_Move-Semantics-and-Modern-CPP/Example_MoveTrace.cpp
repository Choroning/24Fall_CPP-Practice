/**
 * @file    Example_MoveTrace.cpp
 * @brief   A buffer class that follows the rule of five and prints which special member function runs
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <utility>
#include <vector>
#include <cstring>
using namespace std;

class Buffer {
    size_t n;
    int* data;
public:
    explicit Buffer(size_t size) : n(size), data(new int[size]()) {
        cout << "  ctor(" << n << ")" << endl;
    }
    ~Buffer() {                                              // 1. destructor
        if (data) cout << "  dtor(" << n << ")" << endl;
        else      cout << "  dtor(empty)" << endl;
        delete[] data;
    }
    Buffer(const Buffer& o) : n(o.n), data(new int[o.n]) {  // 2. copy constructor
        memcpy(data, o.data, n * sizeof(int));
        cout << "  copy ctor(" << n << ")" << endl;
    }
    Buffer& operator=(const Buffer& o) {                     // 3. copy assignment
        if (this != &o) {
            int* fresh = new int[o.n];
            memcpy(fresh, o.data, o.n * sizeof(int));
            delete[] data;
            data = fresh;
            n = o.n;
        }
        cout << "  copy assign(" << n << ")" << endl;
        return *this;
    }
    Buffer(Buffer&& o) noexcept : n(o.n), data(o.data) {     // 4. move constructor
        o.n = 0;
        o.data = nullptr;                        // leaves the source empty but valid
        cout << "  move ctor(" << n << ")" << endl;
    }
    Buffer& operator=(Buffer&& o) noexcept {                 // 5. move assignment
        if (this != &o) {
            delete[] data;
            data = o.data;
            n = o.n;
            o.data = nullptr;
            o.n = 0;
        }
        cout << "  move assign(" << n << ")" << endl;
        return *this;
    }
    size_t size() const { return n; }
};

Buffer makeBuffer(size_t n) {
    Buffer local(n);
    return local;                                // moved or elided, never copied
}

int main() {
    cout << "1. Buffer a(3); Buffer b = a;" << endl;
    Buffer a(3);
    Buffer b = a;

    cout << "2. Buffer c = std::move(a);" << endl;
    Buffer c = std::move(a);
    cout << "  a.size() = " << a.size() << endl;

    cout << "3. b = makeBuffer(5);" << endl;
    b = makeBuffer(5);

    cout << "4. Buffer d = Buffer(7);" << endl;
    Buffer d = Buffer(7);                        // C++17: no copy and no move

    cout << "5. vector push_back" << endl;
    vector<Buffer> v;
    v.reserve(2);
    v.push_back(Buffer(1));                      // a temporary: moved into the vector
    v.push_back(std::move(c));                   // c is given up explicitly

    cout << "6. end of main" << endl;
    return 0;
}
