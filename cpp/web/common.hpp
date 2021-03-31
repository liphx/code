#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <iostream>

int set_fl(int fd, int flags);
unsigned int rand_sleep(unsigned int max_second);

template<class... Args>
void print(const Args&... args)
{
    std::initializer_list<int> { ([](auto i){ std::cout << i << ' '; }(args), 0)... };
    std::cout << std::endl;
}

#endif // COMMON_HPP_
