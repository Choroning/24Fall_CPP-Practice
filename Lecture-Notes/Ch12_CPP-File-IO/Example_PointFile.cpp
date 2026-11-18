/**
 * @file    Example_PointFile.cpp
 * @brief   Saves objects to a text file and loads them back with the same << and >> operators used for the console
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

struct Point {
    int x = 0, y = 0;
};

ostream& operator<<(ostream& out, const Point& p) {     // works for cout and for ofstream
    return out << p.x << ' ' << p.y;
}

istream& operator>>(istream& in, Point& p) {            // works for cin and for ifstream
    Point t;
    if (in >> t.x >> t.y)
        p = t;
    return in;
}

int main() {
    vector<Point> shape = {{0, 0}, {4, 0}, {4, 3}};
    {
        ofstream fout("shape.txt");
        fout << shape.size() << '\n';            // a count first, then one point per line
        for (const Point& p : shape)
            fout << p << '\n';
    }

    ifstream fin("shape.txt");
    size_t n = 0;
    vector<Point> loaded;
    if (fin >> n) {
        Point p;
        for (size_t i = 0; i < n && fin >> p; i++)
            loaded.push_back(p);
    }
    cout << "loaded " << loaded.size() << " of " << n << " points:";
    for (const Point& p : loaded)
        cout << " (" << p << ")";                // the same operator writes to the screen
    cout << endl;
    return 0;
}
