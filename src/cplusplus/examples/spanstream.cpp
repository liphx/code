#include <array>
#include <iostream>
#include <spanstream>
using namespace std;

int main() {
    array<char, 128> buf;
    ospanstream ss(buf);
    ss << "write " << 123 << " to buf";
    cout << buf.data() << endl;
}
