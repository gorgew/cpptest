#include <any>
#include <iostream>
#include <functional>
#include <vector>

// std::any differentReturn(int a) {
//     if (a > 0) {
//         return std::any {1.0};
//     } else {
//         return std::any {5};
//     }
// }

int main(void) {
    // std::any a = differentReturn(1);
    // if (a.type() == typeid(int)) {
    //     std::cout << "a is an int\n";
    // } else if (a.type() == typeid(double)) {
    //     std::cout << "a is a double\n";
    // } else if (a.type() == typeid(void)) {
    //     std::cout << "a is empty\n";
    // } else {
    //     std::cout << a.type().name() << "\n";
    //     std::cout << typeid(int).name() << "\n";
    // }

    // int x = 5;
    // int y = 6;
    // std::vector<std::reference_wrapper<int>> my_ints {x};
    // x = 7;
    // my_ints.emplace_back(y);
    // y = 0;
    // std::cout << "x: " << my_ints[0] << '\n'; 
    // std::cout << "y: " << my_ints[1] << '\n';
    std::vector<int> my_vec;
    my_vec.emplace_back();
    std::cout << "myvec size " << my_vec.size();
}