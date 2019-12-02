
#ifndef LATTE_FUNCTOR_H
#define LATTE_FUNCTOR_H

#include <either.hpp>

constexpr const class id {
    public: template<class Value>
    constexpr const auto operator()(const Value& x) const noexcept {
        return x;
    }
} id;

#include <stdio.h>

const class log {
    public: template<class Value>
    const auto operator()(const Value& x) const {
        printf("log : %d\n", x);
        return x;
    }
} log;

const class add {
    public: template<class Identity>
    const auto operator()(const Identity& a) const {
        return [&a](const auto& b) {
            return a + b;
        };
    }
} adda;

#include <string>

using namespace latte;

const class is_zero {
    public: const auto operator()(const int& x) const {
        using e = either<std::string, int>;
        return (x == 0) ?
            e::right(0) :
            e::left("ok");
    }
} is_zero;

#include <control.hpp>

using namespace latte::control;

int main() {

    log(1);

    log + 2;

    log * log + 3;
    
    const auto e4 = either<std::string, int>::right(4);

    log * log < e4;

    const auto id2 = [](const auto& x) { return x; };

    const auto elog = either<std::string, class log>::right(log);

    functionalization(5) < elog;

    auto test = either<int, int>::right(5);

    return 0;
}

#endif

