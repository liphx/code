#include <map>
#include <iostream>
using namespace std;

int main()
{
    multimap<string,int> data;
    data.insert(pair<string,int>{"abc",-1});
    data.insert(pair<string,int>{"abc",-1});
    data.insert(pair<string,int>{"abc",-2});
    cout<<data.size()<<endl;
    auto pr = data.equal_range("abc");
    if(pr.first != std::end(data))
        for(auto iter = pr.first;iter != pr.second;++iter)
            cout<< iter->first<<" "<<iter->second<<endl;
    cout<<data.count("abc")<<endl;
    data.erase("abc");
    cout<<data.size()<<endl;

    return 0;
}
