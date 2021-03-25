#include <set>
#include <unordered_set>
#include <iostream>
using namespace std;

int main()
{
    //创建set，默认升序
    set<int> data {1,7,1,9};
    cout<<data.size()<<endl;   //3
    data.insert(0);
    data.erase(1);
    auto iter = data.find(3);
    cout<<*iter<<endl;
    cout<<data.count(1)<<endl;
    cout<<data.count(3)<<endl;

    //multiset
    multiset<int> mdata {1,7,1,9};
    cout<<mdata.size()<<endl;
    mdata.insert(0);
    mdata.erase(1);
    mdata.clear();

    //unordered_set
    unordered_set<int> udata { 1,7,9};
    cout<<udata.bucket_count()<<endl;

    //unorderen_multiset
    unordered_multiset<int> umdata{1,7,1,9};
    auto pr = umdata.equal_range(1);
    if(pr.first != std::end(umdata))
        for(auto iter = pr.first;iter != pr.second;++iter)
            cout<< *iter<<endl;

    return 0;
}
