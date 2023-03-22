#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    std::vector<int> data;
    std::istream_iterator<int> begin(std::cin), end;
    std::copy(begin, end, back_inserter(data));
    std::copy(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, " "));
}
