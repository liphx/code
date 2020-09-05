#include <functional>
#include <iostream>
using namespace std;

int main()
{
    hash<int> hash_int;
    cout<<hash_int(1)<<endl;
    cout<<hash_int(-1)<<endl;

    hash<double> hash_double;
    cout<<hash_double(0.5)<<endl;
    cout<<hash_double(3.75)<<endl;

    hash<string> hash_str;
    cout<<hash_str(string{"123"})<<endl;

    return 0;
}
