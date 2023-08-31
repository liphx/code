#include "liph/liph.h"
using namespace std;

bool less3(int x) { return x < 3; }

void func1() {
    vector<int> vc{1, 2, 3, 4, 5};
    vector<int> vc2(5);
    copy(vc.begin(), vc.end(), vc2.begin());

    vector<int> vc3;
    // 以插入方式替代元素，自动扩大容量
    copy(vc.begin(), vc.end(), back_inserter(vc3));

    // vc vc2 vc3: [1, 2, 3, 4, 5]

    vector<int> vc4;
    // less3(x)为真，则忽略x
    remove_copy_if(vc.begin(), vc.end(), back_inserter(vc4), less3);
    // vc4: [3 4 5]

    vector<int> vc5;
    // less3(x)为真，则用默认值-1替代
    replace_copy_if(vc.begin(), vc.end(), back_inserter(vc5), less3, -1);
    /* print(vc5); */
    //=>-1 -1 3 4 5

    // 在原始序列中改变内容
    replace_if(vc.begin(), vc.end(), less3, -1);
    /* print(vc); */
    //=>-1 -1 3 4 5

    // 流迭代器打印
    copy(vc.begin(), vc.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
    //=>-1 -1 3 4 5
}

void func2() {
    vector<int> data(10);
    // 填充
    fill(data.begin(), data.end(), 7);
    /* print(data); */
    //=>7 7 7 7 7 7 7 7 7 7
    // 填充前n个
    fill_n(data.begin(), 5, 3);
    /* print(data); */
    //=>3 3 3 3 3 7 7 7 7 7

    // 生成
    generate(data.begin(), data.end(), []() { return rand() % 10; });
    /* print(data); */
    //=>3 6 7 5 3 5 6 2 9 1
    // 生成前n个
    generate_n(data.begin() + 2, 5, []() { return -1; });
    /* print(data); */
    //=>3 6 -1 -1 -1 -1 -1 2 9 1
}

void func3() {
    int data[8] = {0, 1, 2, 3, 3, 4, 4, 5};
    cout << count(data, data + 8, 3) << endl;
    //=>2
    cout << count_if(data, data + 8, [](int x) { return x < 3; }) << endl;
    //=>3
}

void func4() {
    vector<int> data(5);
    int i = 1;
    generate_n(data.begin(), 4, [&i]() { return i++; });
    /* print(data); */
    //=>1 2 3 4 0
    vector<int> data_copy(data);
    copy(data_copy.begin(), data_copy.end() - 1, data_copy.begin() + 1);
    /* print(data_copy); */
    //=>1 1 2 3 4
    // why not 1 1 1 1 1 ?

    copy_backward(data.begin(), data.end() - 1, data.end());
    /* print(data); */
    //=>1 1 2 3 4

    // 在原序列倒置
    reverse(data.begin(), data.end());
    /* print(data); */
    //=>4 3 2 1 1

    // 倒置到其他序列，原序列不变
    vector<int> data_reverse(5);
    reverse_copy(data.begin(), data.end(), data_reverse.begin());
    /* print(data); */
    /* print(data_reverse); */
    //=>4 3 2 1 1
    //  1 1 2 3 4

    // 交换
    vector<string> str1{"abc", "bcd"};
    vector<string> str2{"123", "234"};
    swap_ranges(str1.begin(), str1.end(), str2.begin());
    /* print(str1);  //=>123 234 */
    /* print(str2);  //=>abc bcd */

    // 在序列中交换元素
    //[first,mid) 与[mid, end)交换
    rotate(str1.begin(), str1.begin() + 1, str1.end());
    /* print(str1);  //=>234 123 */

    // 原序列不变，生成在目的序列中
    rotate_copy(str1.begin(), str1.begin() + 1, str1.end(), str2.begin());
    /* print(str1);  //=>234 123 */
    /* print(str2);  //=>123 234 */

    // 排列
    string s("abc");
    cout << s << " ";
    while (next_permutation(s.begin(), s.end())) {
        cout << s << " ";
    }
    cout << endl;
    //=>abc acb bac bca cab cba
    string s2("caa");
    cout << s2 << " ";
    while (prev_permutation(s2.begin(), s2.end())) {
        cout << s2 << " ";
    }
    cout << endl;
    //=>caa aca aac

    // 随机排列
    string s3("abcd");
    for (int i = 0; i < 4; i++) {
        shuffle(s3.begin(), s3.end(), std::mt19937{std::random_device{}()});
        cout << s3 << " ";
    }
    cout << endl;
    //=>adbc adbc bcda bcda

    // 划分
    vector<int> num{10, 5, 7, 13, 6, 24, 5};
    vector<int> num_copy(num);
    auto iter = partition(num.begin(), num.end(), [](int x) { return x % 2 != 0; });
    /* print(num);             //=>5 5 7 13 6 24 10 */
    cout << *iter << endl;  //=>6
    stable_partition(num_copy.begin(), num_copy.end(), [](int x) { return x % 2 != 0; });
    /* print(num_copy);  //=>5 7 13 5 10 6 24 */
}

void func5() {
    // 线性查找
    vector<int> data{1, 1, 2, -1, 5};
    auto iter = find(data.begin(), data.end(), -1);
    if (iter != data.end()) cout << *iter << endl;
    //=>-1

    iter = find_if(data.begin(), data.end(), [](int x) { return x > 2; });
    if (iter != data.end()) cout << *iter << endl;
    //=>5

    iter = adjacent_find(data.begin(), data.end());
    if (iter != data.end()) cout << *iter << endl;
    //=>1

    iter = adjacent_find(data.begin(), data.end(), [](int x, int y) { return x + y > 3; });
    if (iter != data.end()) cout << *iter << endl;
    //=>-1

    vector<string> str1{"eee", "bcd"};
    vector<string> str2{"bcd", "cde"};
    auto iter2 = find_first_of(str1.begin(), str1.end(), str2.begin(), str2.end());
    if (iter2 != str1.end()) cout << *iter2 << endl;
    //=>bcd

    iter2 = find_first_of(str1.begin(), str1.end(), str2.begin(), str2.end(), [](string x, string y) { return x > y; });
    if (iter2 != str1.end()) cout << *iter2 << endl;
    //=>eee

    // search find_end 查找序列2是否在序列1中出现

    iter2 = search(str1.begin(), str1.end(), str2.begin(), str2.begin());
    if (iter2 != str1.end()) cout << *iter2 << endl;
    //=>eee

    iter2 = search(str1.begin(), str1.end(), str2.begin(), str2.begin() + 1);
    if (iter2 != str1.end()) cout << *iter2 << endl;
    //=>bcd

    iter2 = search(str1.begin(), str1.end(), str2.begin(), str2.end());
    if (iter2 != str1.end()) cout << *iter2 << endl;
    // null

    iter2 = find_end(str1.begin(), str1.end(), str2.begin(), str2.begin() + 1);
    if (iter2 != str1.end()) cout << *iter2 << endl;
    //=>bcd

    vector<bool> b{0, 1, 1, 0, 0, 0};
    auto iter3 = search_n(b.begin(), b.end(), 2, true);
    if (iter3 != b.end()) cout << *iter3 << endl;  //=>1
    iter3 = search_n(b.begin(), b.end(), 3, false);
    if (iter3 != b.end()) cout << *iter3 << endl;  //=>0

    string s("defabc");
    auto iter4 = min_element(s.begin(), s.end());
    cout << *iter4 << endl;  //=>a
    iter4 = max_element(s.begin(), s.end());
    cout << *iter4 << endl;  //=>f
}

void func6() {
    vector<int> a{1, 2, 3, 4};
    vector<int> b{0, 1, 2, 3};
    cout << equal(a.begin(), a.end(), b.begin()) << endl;
    //=>0
    cout << equal(a.begin(), a.end() - 1, b.begin() + 1) << endl;
    //=>1

    string s1("abcd");
    string s2("abce");
    cout << lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end()) << endl;
    //=>1
    cout << lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end() - 1) << endl;
    //=>0

    vector<int> c{1, 2, 3};
    vector<int> d{1, 3, 4};
    auto iter_pair = mismatch(c.begin(), c.end(), d.begin());
    if (iter_pair.first != c.end() && iter_pair.second != d.end())
        cout << *iter_pair.first << " " << *iter_pair.second << endl;
    //=>2 3
}

void func7() {
    vector<int> data{1, 1, -1, 0, 1, 2};
    auto iter = remove(data.begin(), data.end(), -1);
    /* print(data); */
    //=>1 1 0 1 2 2
    data.erase(iter);
    /* print(data); */
    //=>1 1 0 1 2

    data.erase(unique(data.begin(), data.end()), data.end());
    /* print(data); */
    //=>1 0 1 2

    sort(data.begin(), data.end());
    data.erase(unique(data.begin(), data.end()), data.end());
    /* print(data); */
    //=>0 1 2
}

void func8() {
    vector<int> data(10);
    generate(data.begin(), data.end(), []() { return rand() % 20; });
    /* print(data); */
    //=>3 6 17 15 13 15 6 12 9 1
    vector<int> data_copy(data), data_copy2(data), data_copy3(data);
    sort(data.begin(), data.end());
    /* print(data); */
    //=>1 3 6 6 9 12 13 15 15 17
    stable_sort(data_copy.begin(), data_copy.end());
    /* print(data_copy); */
    //=>1 3 6 6 9 12 13 15 15 17

    partial_sort(data_copy2.begin(), data_copy2.begin() + 5, data_copy2.end());
    /* print(data_copy2); */
    //=>1 3 6 6 9 17 15 15 13 12

    nth_element(data_copy3.begin(), data_copy3.begin() + 5, data_copy3.end());
    /* print(data_copy3); */
    //=>6 3 1 6 9 12 13 15 15 17

    int a[3] = {1, 2, 3};
    int b[4] = {0, 1, 3, 4};
    vector<int> array;
    merge(a, a + 3, b, b + 4, back_inserter(array));
    /* print(array); */
    //=>0 1 1 2 3 3 4

    rotate(array.begin(), array.begin() + 4, array.end());
    /* print(array); */
    //=>3 3 4 0 1 1 2
    inplace_merge(array.begin(), array.begin() + 3, array.end());
    /* print(array); */
    //=>0 1 1 2 3 3 4

    vector<int> array2{0, 1, 1};
    cout << includes(array.begin(), array.end(), array2.begin(), array2.end()) << endl;
    //=>1

    vector<int> array3{-2, -1, 0, 1, 1};
    set_union(array.begin(), array.end(), array3.begin(), array3.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
    //=>-2 -1 0 1 1 2 3 3 4

    set_intersection(array.begin(), array.end(), array3.begin(), array3.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
    //=>0 1 1

    set_difference(array.begin(), array.end(), array3.begin(), array3.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
    //=>2 3 3 4

    set_symmetric_difference(
            array.begin(), array.end(), array3.begin(), array3.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
    //=>-2 -1 2 3 3 4
}

void func9() {
    vector<int> data{14, 5, 13, 26, 4, 18, 47, 26};

    // 创建小顶堆
    make_heap(data.begin(), data.end(), greater<int>());
    /* print(data); */
    //=>4 5 13 26 14 18 47 26

    // 创建大顶堆
    make_heap(data.begin(), data.end());
    /* print(data); */
    //=>47 26 18 26 14 4 13 5

    // 添加
    data.push_back(37);
    push_heap(data.begin(), data.end());
    /* print(data); */
    //=>47 37 18 26 14 4 13 5 26

    // 删除
    pop_heap(begin(data), end(data));
    data.pop_back();
    /* print(data); */
    //=>37 26 18 26 14 4 13 5

    // 判断是否依旧为堆
    cout << is_heap(begin(data), end(data)) << endl;
    //=>1

    // 排序
    if (is_heap(begin(data), end(data))) sort_heap(begin(data), end(data));
    /* print(data); */
    //=>4 5 13 14 18 26 26 37
}

void func10() {
    vector<int> data{1, 2, 3, 4, 5};
    for_each(data.begin(), data.end(), [](int x) { cout << x << " "; });
    cout << endl;
    //=>1 2 3 4 5
    for_each(data.begin(), data.end(), [](int) { return 0; });
    /* print(data); */
    //=>1 2 3 4 5

    vector<int> data2(5);
    transform(data.begin(), data.end(), data2.begin(), [](int x) { return x + 1; });
    /* print(data2); */
    //=>2 3 4 5 6
    vector<int> data3(5);
    transform(data.begin(), data.end(), data2.begin(), data3.begin(), [](int x, int y) { return x * y; });
    /* print(data3); */
    //=>2 6 12 20 30
}

void func11() {
    vector<int> data{1, 2, 3, 4, 5};
    cout << accumulate(data.begin(), data.end(), 0) << endl;
    //=>15
    cout << accumulate(data.begin(), data.end(), 0, [](int x, int y) { return x + y * y; }) << endl;
    //=>55

    // 内积
    vector<int> data2(data);
    cout << inner_product(data.begin(), data.end(), data2.begin(), 0) << endl;
    //=>55
    // 自定义op+ op*
    cout << inner_product(
                    data.begin(), data.end(), data2.begin(), 0, [](int x, int y) { return x + y; },
                    [](int x, int y) { return x * y; })
         << endl;
    //=>55

    vector<int> data3(5);
    partial_sum(data.begin(), data.end(), data3.begin());
    /* print(data3); */
    //=>1 3 6 10 15
    partial_sum(data.begin(), data.end(), data3.begin(), multiplies<int>());
    /* print(data3); */
    //=>1 2 6 24 120

    adjacent_difference(data.begin(), data.end(), data3.begin());
    /* print(data3); */
    //=>1 1 1 1 1
    adjacent_difference(data.begin(), data.end(), data3.begin(), multiplies<int>());
    /* print(data3); */
    //=>1 2 6 24 120
}

void func12() {
    pair<string, int> a = make_pair(string("Hello"), 12);
    cout << a.first << " " << a.second << endl;
    //=>Hello 12

    vector<int> b(10);
    list<int> c(15);
    cout << distance(b.end(), b.begin()) << endl;  //=>-10
    cout << distance(c.begin(), c.end()) << endl;  //=>15

    vector<int> d{1, 2, 3};
    list<int> e;
    copy(d.begin(), d.end(), back_inserter(e));
    copy(e.begin(), e.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
    //=>1 2 3
    copy(d.begin(), d.end(), front_inserter(e));
    copy(e.begin(), e.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
    //=>3 2 1 1 2 3
    auto iter = e.begin();
    advance(iter, 2);
    copy(d.begin(), d.end(), inserter(e, iter));
    copy(e.begin(), e.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
    //=>3 2 1 2 3 1 1 2 3

    string s1("abc"), s2("bc");
    cout << min(s1, s2, [](string x, string y) { return x.size() < y.size(); }) << endl;
    //=>bc
    cout << max(s1, s2, [](string x, string y) { return x.at(0) < y.at(0); }) << endl;
    //=>bc

    swap(s1, s2);
    cout << s1 << " " << s2 << endl;
    //=>bc abc

    vector<int> data{0, 1};
    auto it1 = data.begin();
    auto it2 = data.begin() + 1;
    cout << distance(it1, it2) << endl;
    //=>1
    iter_swap(it1, it2);
    cout << *it1 << " " << *it2 << endl;
    //=>1 0
    cout << distance(it1, it2) << endl;
    //=>1
}

int main() {
    func1();
    func2();
    func3();
    func4();
    func5();
    func6();
    func7();
    func8();
    func9();
    func10();
    func11();
    func12();
}
