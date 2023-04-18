// -O0

void f(int n = 0) {
    char dummy[1024];
    f(++n);
}

int main() { f(); }
