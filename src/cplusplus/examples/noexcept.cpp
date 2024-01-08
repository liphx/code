// https://en.cppreference.com/w/cpp/language/noexcept_spec

void func() noexcept { throw 42; }  // call std::terminate if throw in noexcept function

int main() {
    try {
        func();
    } catch (...) {
        // no use, still terminate
    }
}
