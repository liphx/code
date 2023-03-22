#include <iostream>
#include <map>
using namespace std;

int main() {
    //创建map 元素是pair类型
    map<string, int> data1;
    map<string, int> data2{{"la", 10}, {"lb", 20}, {"lc", 30}};
    map<string, int> data3{make_pair("la", 10), make_pair("lb", 20), make_pair("lc", 30)};
    map<string, int> data4{data3};

    //插入数据
    auto ret_pr = data1.insert(pair<string, int>{"abc", -1});
    cout << ret_pr.first->first << " " << ret_pr.first->second << endl;
    cout << ret_pr.second << endl;
    if (!data1.count("abd")) data1.insert(pair<string, int>{"abd", 1});
    cout << data1.count("abd") << endl;
    data1.insert({{"abe", 2}, {"abd", 0}});  //只插入了一个元素
    cout << data1.size() << endl;
    data1.emplace(pair<string, int>{"abf", 10});

    //访问元素
    cout << data1.at("abf") << endl;
    cout << data1["abf"] << endl;
    data1["abg"] = 5;
    cout << data1.at("abg") << endl;
    auto iter = data1.find(string("abg"));
    if (iter == end(data1))
        cout << "Not found" << endl;
    else
        cout << iter->second << endl;

    //删除元素
    cout << data1.erase("abg") << endl;  //返回删除元素的个数
    cout << data1.erase("abh") << endl;
    data1.clear();
    cout << data1.size() << endl;

    return 0;
}
