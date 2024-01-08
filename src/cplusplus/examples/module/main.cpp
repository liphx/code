import vector;

#include <iostream>

int main() {
    vector v1(10);
    vector v2(10);
    for (int i = 0; i < v1.size(); ++i) {
        v1[i] = v2[i] = i;
    }
    // print(v1);
    // print(v2);
    std::cout << std::boolalpha << (v1 == v2) << std::endl;
}
