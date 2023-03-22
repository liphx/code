int binary_search(int a[], int n, int val) {
    int i = 0, j = n - 1, mid;
    while (i <= j) {
        /* mid = (i + j) / 2;  // bug */
        mid = i + (j - i) / 2;
        if (a[mid] == val) {
            return mid;
        }
        if (a[mid] < val) {
            i = mid + 1;
        } else {
            j = mid - 1;
        }
    }
    return -1;
}

#include <iostream>

int main() {
    int arr[]{-9, 0, 1, 7};
    std::cout << binary_search(arr, sizeof(arr) / sizeof(int), 1) << std::endl;
}
