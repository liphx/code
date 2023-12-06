template <class T>
void ignore(const T&) {}

// C style
static inline void ignore_(int, ...) {}
#define IGNORE(x) ignore_(0, (x))

[[nodiscard]] int f() { return 42; }

int main() {
    f();  // Wunused
    ignore(f());
    IGNORE(f());
}
