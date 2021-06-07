#include <functional>
#include <iostream>

std::function<int(int, int)> add() {
    return [](int a, int b){
        return a + b;
    };
}

auto subtract() {
    return [](int a, int b){
        return a - b;
    };
}

int main(void) {
    std::function<int(int, int)> my_add = add();
    std::cout << my_add(5, 5) <<'\n';

    int x = 5;
    auto my_lambda = [&](int y) {
        return y;
    };

    std::function<int(void)> my_function = [&]() { return my_lambda(x);};
    return 0;
}