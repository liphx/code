#include <iostream>
#include <vector>

using namespace std;

template <class T>
void print(vector<T>& vc) {
    cout << "[";
    bool first = true;
    for (const auto& x : vc) {
        if (!first) cout << ",";
        cout << x;
        first = false;
    }
    cout << "]\n";
}

int main() {
    {
        struct A {
            A() { cout << "A()\n"; }
            A(const A&) { cout << "A(A&)\n"; }
        };

        vector<A> vc;
        vc.reserve(2);

        A a;
        vc.push_back(a);

        vc.emplace_back();
    }
    {
            // vector<int&> vc;
            // https://stackoverflow.com/questions/922360/why-cant-i-make-a-vector-of-references

            // vector<const int> d{0, 0, 1, 1};
            // cout << d.size() << endl;
            // cout << d[0] << endl;
            // d[0] = 1;
            // d.emplace_back(0);
            // const int a = 0;
            // d.emplace_back(a);
    }

    {
        vector<int> data1;
        cout << data1.size() << endl;      // 0
        cout << data1.capacity() << endl;  // 0

        data1.reserve(20);
        cout << data1.size() << endl;      // 0
        cout << data1.capacity() << endl;  // 20

        vector<int> data2{1, 2, 3, 4, 5};

        vector<int> data3(10);
        cout << data3.size() << endl;      // 10
        cout << data3.capacity() << endl;  // 10
        print(data3);                      // [0,0,0,0,0,0,0,0,0,0]

        vector<int> data4(5, -1);
        print(data4);  // [-1,-1,-1,-1,-1]

        vector<int> data5{1, 2};
        data5.resize(3);
        print(data5);  // [1,2,0]
        data5.resize(5, -1);
        print(data5);  // [1,2,0,-1,-1]
        data5.resize(4);
        print(data5);  // [1,2,0,-1]

        cout << data5[3] << endl;     // -1
        cout << data5.at(0) << endl;  // 1

        data5.front()++;
        cout << data5[0] << endl;  // 2

        data5.back()--;
        cout << *(data5.end() - 1) << endl;  // -2

        (void)data5.data();

        data5.push_back(100);
        cout << data5.size() << endl;  // 5
        cout << data5.back() << endl;  // 100

        vector<string> data6;
        string str = "hello";
        data6.emplace_back(str, 0, 3);
        cout << data6[0] << endl;  // hel

        auto iter = data6.begin();
        data6.emplace(iter, str, 1, 3);
        print(data6);  // [ell,hel]

        data6.insert(data6.begin(), "00");
        data6.insert(data6.end(), {"11", "22"});
        print(data6);  // [00,ell,hel,11,22]

        data6.pop_back();

        data6.shrink_to_fit();

        data6.erase(data6.begin());

        data6.clear();
        cout << data6.size() << endl;      // 0
        cout << data6.capacity() << endl;  // 4
        data6.shrink_to_fit();
        cout << data6.capacity() << endl;  // 0
    }
}
