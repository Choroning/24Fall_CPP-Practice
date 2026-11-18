/**
 * @file    Example_BinaryRecords.cpp
 * @brief   Stores fixed-size records with write(), reads one with seekg() and read(), and updates one with seekp()
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <fstream>
#include <cstddef>
using namespace std;

struct Record {
    int id;
    char name[10];
    double score;
};

int main() {
    const Record table[3] = {{101, "Kim", 93.5}, {102, "Lee", 81.0}, {103, "Park", 72.25}};
    cout << "sizeof(Record) = " << sizeof(Record) << ", offsets: id " << offsetof(Record, id)
         << ", name " << offsetof(Record, name) << ", score " << offsetof(Record, score) << endl;

    ofstream fout("records.bin", ios::out | ios::binary);
    fout.write(reinterpret_cast<const char*>(table), sizeof(table));   // the bytes of 3 records
    fout.close();

    fstream file("records.bin", ios::in | ios::out | ios::binary);
    if (!file) {
        cerr << "cannot open records.bin" << endl;
        return 1;
    }
    file.seekg(0, ios::end);
    cout << "file size = " << file.tellg() << " bytes" << endl;

    Record r;
    file.seekg(2 * sizeof(Record), ios::beg);    // jumps to record index 2
    file.read(reinterpret_cast<char*>(&r), sizeof(r));
    cout << "record 2: " << r.id << ' ' << r.name << ' ' << r.score
         << " (gcount " << file.gcount() << ", tellg " << file.tellg() << ")" << endl;

    r.score = 88.0;                              // changes the score of record 2
    file.seekp(2 * sizeof(Record), ios::beg);
    file.write(reinterpret_cast<const char*>(&r), sizeof(r));
    file.flush();

    file.seekg(0, ios::beg);
    while (file.read(reinterpret_cast<char*>(&r), sizeof(r)))
        cout << r.id << ' ' << r.name << ' ' << r.score << endl;
    cout << "last gcount = " << file.gcount() << endl;
    return 0;
}
