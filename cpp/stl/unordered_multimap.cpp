#include <unordered_map>
#include <iostream>
using namespace std;

int main()
{
    unordered_multimap<string,int> data {{"001",1},{"123",6},{"001",2},{"001",1} };
    cout<<data.size()<<endl;
    data.insert(pair<string,int> { "113",5} );
    cout<<data.count("001")<<endl;

    auto iter = data.find("113");
    if(iter == end(data))
        cout <<"Not found"<<endl;
    else
        cout<<iter->second<<endl;

    auto pr = data.equal_range("001");
    if(pr.first != std::end(data))
        for(auto iter = pr.first;iter != pr.second;++iter)
            cout<< iter->first<<" "<<iter->second<<endl;

    data.erase("001");
    cout<<data.size()<<endl;

    return 0;
}
