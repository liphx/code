#include "liph/prime_sieve.h"

#include "gtest/gtest.h"

TEST(prime_sieve, prime_sieve) {
    std::vector<int> nums{1, 2, 4, 9, 17, 21, 119, 997, 1001, 8388617, 8388601};
    std::vector<bool> ans{0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0};
    liph::prime_sieve prime(100);
    for (int i = 0; i < nums.size(); i++) {
        EXPECT_EQ(prime.is_prime(nums[i]), ans[i]);
    }
}
