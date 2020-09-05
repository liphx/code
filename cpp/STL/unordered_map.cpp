#include <unordered_map>
#include <iostream>
using namespace std;

int main()
{
    //初始化，可以提供哈希函数和operator ==
    unordered_map<string,int> data { {"001",0},{"002",1},{"003",-1} };

    //插入元素
    data.insert(pair<string,int>{ "004", -2} );
    cout<<data.size()<<endl;
    cout<<data.bucket_count()<<endl; //容器格子数
    auto iter = data.insert(pair<string,int>{ "004", -5} );
    cout<<iter.second<<endl;    //插入失败
    data.rehash(7);           //分配格子数并重新哈希
    cout<<data.bucket_count()<<endl;

    //获取元素
    cout<<data["004"]<<endl;
    data["005"] = -10;
    cout<<data["005"]<<endl;
    cout<<data.at("005")<<endl;

    //访问格子
    size_t i = data.bucket("005");
    cout<<i<<" "<<data.bucket_size(i)<<endl;
    auto pr = data.begin(i); //begin()的重载版本，获取第1个格子
    for(; pr != data.end(i); ++pr) //输出格子内的元素
        cout<<pr->first<<" "<<pr->second<<endl;

    //删除元素
    cout<<data.erase("006")<<endl;
    cout<<data.erase("005")<<endl;
    data.clear();
    cout<<data.size()<<endl;

    return 0;
}
