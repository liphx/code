#include "liph/liph.h"
using namespace std;

const int max_i = 10000000;
bool is_prime[max_i];

int main() {
    memset(is_prime, 1, max_i);
    is_prime[0] = is_prime[1] = 0;

    for (int i = 2; i < max_i; i++) {
        if (is_prime[i]) {
            for (int j = i * 2; j < max_i; j += i) is_prime[j] = false;
        }
    }

    int nums[] = {1, 2, 4, 9, 17, 21, 119};
    for (int num : nums) cout << is_prime[num] << " ";
    // 0 1 0 0 1 0 0
}
