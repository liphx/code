#include "json.hpp"
using namespace liph;

#include <iostream>
using namespace std;

int main()
{
    json j = json::parse("{\"name\":\"liph\",\"age\":23}");
    cout << j.dump() << endl;
}
