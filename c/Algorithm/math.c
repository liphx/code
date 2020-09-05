#include <stdio.h>

static long long _factorial(int ans, int n){
    if (n <= 1)
        return ans;
    return _factorial(ans * n, n-1);
}

long long factorial(int n)
{
    return _factorial(1, n);
}

int main()
{
    int i = 0;
    for (; i < 16; i++)
        printf("%lld ", factorial(i)); /* c99 */
}