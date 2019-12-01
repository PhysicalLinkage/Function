#include <either.hpp>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <string>
#include <identity.hpp>
#include <functor.hpp>

using namespace latte;

int main() {
    
    const auto id = [](const auto x) { return x; };
    
    const auto log = [](const std::string& x) { printf("log : %s\n", x.c_str()); return x; };

    const auto log_int = [](const int x) { printf("log : %d\n", x); return x; };

    const auto add = [](const auto a) { return [a](const auto b) { return a + b; }; };

    const auto to_string = [](const auto x) { return "success : " + std::to_string(x); };
    
    const auto error = [](const auto x) { return std::string("error : ") + x; };

    const auto result = [](const auto x) { printf("result : %s.\n", x.c_str()); };

    const auto start = [](const auto x) { printf("--------%s--------\n", x); };

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
        .fmap(log)
        .merge(id)(id));

    merge(id)(id)(is_zero(0));

    //log > merge < id < id < is_zero(0);

    printf(">>>>>>>>>>>>>>\n");

    
    // Identity
    log_int < 10;

    log_int > log_int;

    log_int > log_int < 10;


    // functor
    log_int <= either<int, int>{right{10}};

    //log_int >= either<int, int(*)(int)>{right{log_int}};


    printf(">>>>>>>>>>>>>>\n");

    return 0;

}

