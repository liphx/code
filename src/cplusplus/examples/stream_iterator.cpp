#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    {
        std::ostream_iterator<std::string> out{std::cout};
        *out = "hello";
        *out = ", world\n";
    }
    {
        std::ostream_iterator<std::string> out{std::cout, "\n"};
        *out = "hello";
        ++out;
        *out = "world";
    }
    {
        std::vector<int> data;
        // end of input: default istream_iterator
        std::istream_iterator<int> begin(std::cin), end;
        std::copy(begin, end, back_inserter(data));
        std::copy(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, " "));
    }
}
