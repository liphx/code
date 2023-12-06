export module vector;

export class vector {
public:
    vector(int);
    double& operator[](int);
    const double& operator[](int) const;
    int size() const;

private:
    double *ptr;
    int sz;
};

vector::vector(int s) : ptr{new double[s]}, sz{s} {}
double& vector::operator[](int i) { return ptr[i]; }
const double& vector::operator[](int i) const { return ptr[i]; }
int vector::size() const { return sz; }

export bool operator==(const vector& v1, const vector& v2) {
    if (v1.size() != v2.size()) return false;
    for (int i = 0; i < v1.size(); ++i)
        if (v1[i] != v2[i]) return false;
    return true;
}

// #include <iostream>
// using namespace std;  // local to the module

// export void print(const vector& v) {
//     for (int i = 0; i < v.size(); ++i) cout << v[i] << endl;
// }
