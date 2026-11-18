/**
 * @file    Example_Manipulators.cpp
 * @brief   Formats a receipt table with the manipulators of <iostream> and <iomanip>
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

struct Item {
    string name;
    int quantity;
    double price;
};

int main() {
    const Item items[] = {{"Notebook", 3, 1.5}, {"Pen", 12, 0.75}, {"Backpack", 1, 24.99}};

    cout << left << setw(10) << "Item" << right << setw(5) << "Qty"
         << setw(10) << "Price" << setw(10) << "Total" << '\n';
    cout << setfill('-') << setw(35) << "" << setfill(' ') << '\n';

    cout << fixed << setprecision(2);            // these two settings stay until changed
    double sum = 0;
    for (const Item& it : items) {
        double total = it.quantity * it.price;
        sum += total;
        cout << left << setw(10) << it.name << right << setw(5) << it.quantity
             << setw(10) << it.price << setw(10) << total << '\n';
    }
    cout << left << setw(25) << "Sum" << right << setw(10) << sum << '\n';

    cout << defaultfloat << setprecision(6);     // undoes fixed and setprecision(2)
    cout << showbase << hex << 255 << ' ' << oct << 255 << ' ' << dec << 255 << noshowbase << '\n';
    cout << boolalpha << (sum > 50) << ' ' << noboolalpha << (sum > 50) << '\n';
    cout << '[' << setw(7) << internal << -42 << "] [" << setw(7) << left << -42 << "]\n" << right;
    return 0;
}
