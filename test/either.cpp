#include <either.hpp>

#include <iostream>
#include <algorithm>
#include <vector>

using namespace latte;

int main() {

    const auto id = [](const auto x) { return x; };
    
    const auto log = [](const std::string& x) { std::cout << "log : " << x << "\n"; return x; };

    const auto log_int = [](const int x) { std::cout << "log : " << std::to_string(x) << "\n"; return x; };

    const auto add = [](const auto a) { return [a](const auto b) { return a + b; }; };

    const auto to_string = [](const auto x) { return "success : " + std::to_string(x); };
    
    const auto error = [](const auto x) { return "error : " + x; };

    const auto result = [](const auto x) { std::cout << "result : " + x + ".\n"; };

    const auto start = [](const auto x) { std::cout << std::string("--------") + x + "--------\n"; };

    const auto is_range = [](const size_t min, const size_t value, const size_t max) {
        return (min <= value && value < max);
    };

    const auto get = [is_range](const std::vector<int>& vector, const size_t index) {
        using E = either<std::string, int>;
        return is_range(0, index, vector.size()) ?
            E{right{vector[index]}} :
            E{left{"'" + std::to_string(index) + "' is out of range"}};
    };

    const auto is_zero = [](const auto x) {
        using S = std::string;
        using E = either<S, S>;
        return !(x == 0) ? E{left{S{"false"}}} : E{right{S{"true"}}};
    };
    
    const std::vector<int> array{0, 1, 2, 3, 4, 5};

    start("get(array, 4)");
    result(get(array, 4)
        .fmap(log_int)
        .merge(error)(to_string));
    
    start("get(array, 100)");
    result(get(array, 100)
        .fmap(log_int)
        .merge(error)(to_string));

    start("is_0(0)");
    result(is_zero(0)
        .fmap(log)
        .merge(id)(id));
    start("is_0(10)");
    result(is_zero(10)
        .fmap(log)
        .merge(id)(id));

    start("get bind is_zero");
    result(get(array, 3)
        .fmap(log_int)
        .fmap(add(-3))
        .fmap(log_int)
        .bind(is_zero)
        .fmap(log)
        .merge(id)(id));

    start("get bind is_zero take2");
    result(get(array, 2)
        .fmap(log_int)
        .bind(is_zero)
        .reverse()
        .fmap(log)
        .merge(id)(id));

    return 0;

}


