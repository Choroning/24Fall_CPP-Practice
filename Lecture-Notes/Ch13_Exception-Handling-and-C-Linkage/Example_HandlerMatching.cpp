/**
 * @file    Example_HandlerMatching.cpp
 * @brief   Shows which handler catches each thrown type, including the absence of arithmetic conversions
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <string>
using namespace std;

struct Base { virtual ~Base() = default; virtual string name() const { return "Base"; } };
struct Derived : Base { string name() const override { return "Derived"; } };

void test(int kind) {
    try {
        switch (kind) {
        case 0: throw 42;                        // int
        case 1: throw 3.5f;                      // float: no conversion to double
        case 2: throw "message";                 // const char*
        case 3: throw Derived();                 // matches const Base&
        case 4: throw string("text");            // no handler below matches std::string
        }
    } catch (int n) {
        cout << kind << ": int " << n << endl;
    } catch (double d) {
        cout << kind << ": double " << d << endl;
    } catch (const char* s) {
        cout << kind << ": const char* " << s << endl;
    } catch (const Base& b) {
        cout << kind << ": Base& holding " << b.name() << endl;
    } catch (...) {
        cout << kind << ": caught by ..." << endl;
    }
}

int main() {
    for (int k = 0; k <= 4; k++)
        test(k);
    return 0;
}
