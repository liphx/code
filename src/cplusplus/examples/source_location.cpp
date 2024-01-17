#include <iostream>
#include <source_location>
using namespace std;

int main() {
    const source_location loc = source_location::current();
    cout << "file: " << loc.file_name() << '\n';
    cout << "func: " << loc.function_name() << '\n';
    cout << "line: " << loc.line() << '\n';
    cout << "column: " << loc.column() << '\n';
}
