/**
 * @file    Example_UniqueOwnership.cpp
 * @brief   Passes unique_ptr ownership through functions and closes a FILE* with a custom deleter
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
using namespace std;

struct Task {
    string title;
    explicit Task(string t) : title(std::move(t)) {}
    ~Task() { cout << "  ~Task(" << title << ")" << endl; }
};

unique_ptr<Task> createTask(const string& t) {   // returns ownership to the caller
    return make_unique<Task>(t);
}

void show(const Task& t) {                       // only looks at the object
    cout << "  show " << t.title << endl;
}

void consume(unique_ptr<Task> t) {               // takes ownership; t dies at the end
    cout << "  consume " << t->title << endl;
}

struct FileCloser {
    void operator()(FILE* f) const {
        cout << "  fclose called" << endl;
        fclose(f);
    }
};

int main() {
    auto t1 = createTask("write report");
    show(*t1);

    vector<unique_ptr<Task>> queue;
    queue.push_back(std::move(t1));              // t1 becomes null
    queue.push_back(createTask("send mail"));
    cout << "  t1 is " << (t1 ? "not null" : "null") << ", queue size " << queue.size() << endl;

    consume(std::move(queue[0]));                // destroyed inside consume
    cout << "  after consume" << endl;

    {
        unique_ptr<FILE, FileCloser> log(fopen("log.txt", "w"));
        if (log)
            fputs("first line\n", log.get());    // get() gives the raw pointer for C functions
    }                                            // FileCloser runs here
    cout << "  end of main" << endl;
    return 0;
}
