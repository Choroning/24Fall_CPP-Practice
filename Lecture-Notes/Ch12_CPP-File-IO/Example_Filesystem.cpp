/**
 * @file    Example_Filesystem.cpp
 * @brief   Creates, lists, measures, and removes files with std::filesystem (C++17)
 * @author  박철원 (Cheolwon Park)
 * @date    2026-11-19
 */

#include <iostream>
#include <fstream>
#include <filesystem>
#include <system_error>
namespace fs = std::filesystem;
using namespace std;

int main() {
    fs::path dir = "fs_demo";
    fs::create_directories(dir / "logs");        // creates both levels if missing
    { ofstream(dir / "a.txt") << "alpha\n"; }    // a temporary ofstream closes at the ';'
    { ofstream(dir / "logs" / "b.log") << "beta beta\n"; }

    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
        cout << entry.path().generic_string();
        if (entry.is_regular_file())
            cout << "  " << entry.file_size() << " bytes";
        cout << endl;
    }

    fs::path p = dir / "logs" / "b.log";
    cout << "parent=" << p.parent_path().generic_string() << " stem=" << p.stem().string()
         << " extension=" << p.extension().string() << endl;

    error_code ec;
    auto size = fs::file_size(dir / "none.txt", ec);   // the error_code version does not throw
    cout << "missing file: " << ec.message() << " (" << (size == static_cast<uintmax_t>(-1)) << ")" << endl;

    cout << "removed " << fs::remove_all(dir) << " entries" << endl;
    cout << "exists after remove_all: " << fs::exists(dir) << endl;
    return 0;
}
