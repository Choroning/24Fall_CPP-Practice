/**
 * @file    Example_ConstructorFailure.cpp
 * @brief   When a member constructor throws, the members already built are destroyed and the object's destructor never runs
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

struct Part {
    string name;
    explicit Part(string n) : name(n) { cout << "  Part " << name << " built" << endl; }
    ~Part() { cout << "  Part " << name << " destroyed" << endl; }
};

struct Engine {
    Engine() {
        cout << "  Engine throws" << endl;
        throw runtime_error("engine failure");
    }
    ~Engine() { cout << "  ~Engine" << endl; }
};

struct Car {
    Part body;                                   // members are built in declaration order
    Engine engine;
    Part wheels;
    Car() try : body("body"), engine(), wheels("wheels") {
        cout << "  Car built" << endl;
    } catch (const exception& e) {               // a function-try-block sees initializer exceptions
        cout << "  Car's handler: " << e.what() << endl;
    }                                            // the exception is rethrown automatically here
    ~Car() { cout << "  ~Car" << endl; }
};

int main() {
    try {
        Car c;
        cout << "  never printed" << endl;
    } catch (const exception& e) {
        cout << "main caught: " << e.what() << endl;
    }
    return 0;
}
