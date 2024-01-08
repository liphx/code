#include <iostream>

enum class E : int {
    zero = 0,
    one = 1,
};

enum class Color { red, yellow, green };
enum class Light { red, yellow, green };

Light& operator++(Light& light) {
    using enum Light;
    switch (light) {
    case red:
        return light = yellow;
    case yellow:
        return light = green;
    case green:
        return light = red;
    }
}

enum plain { zero, one, two };

int main() {
    E e = E::zero;
    // int i = e;
    int i = static_cast<int>(e);
    int j = int(e);
    std::cout << i << std::endl;  // 0
    std::cout << j << std::endl;  // 0

    // error
    // Color c = red;
    // Color c = Light::red;
    // Color c = 0;
    Color c = Color::red;
    Color c2 = Color(0);
    Color c3 = Color{0};
    Color c4{0};
    int x = int(c4);
    int y = int(Light::green);
    std::cout << x << " " << y << std::endl;  // 0 2

    auto light{Light::green};
    std::cout << int(light) << std::endl;  // 2
    ++light;
    std::cout << int(light) << std::endl;  // 0

    int n = zero;
    std::cout << n << std::endl;  // 0
}
