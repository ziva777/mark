#include <iostream>

int main(int argc, char const *argv[]) {
    auto lambda = [](auto x, auto y) {return x + y;};
    std::cout << "Hi there! It's " << lambda(1, 3) << " here." << std::endl;
    return 0;
}