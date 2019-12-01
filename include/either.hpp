
#ifndef LATTE_EITHER_H
#define LATTE_EITHER_H

#include <utility>

namespace latte {

template<class T> struct left  { 
    const T value;
    left(const T& value_) : value{value_} {}
};
template<class T> struct right { 
    const T value;
    right(const T& value_) : value{value_} {} 
};

template<class T> struct wrapper {
    const T value;
    wrapper(const T& a) : value(a) {}
};

template<class L, class R>
class either {

    const bool is_left_;
    const union { const left<L> left_; const right<R> right_; };

    public: either(const left <L>& left)  : is_left_{true},  left_ {left } {}
    public: either(const right<R>& right) : is_left_{false}, right_{right} {}
    public: either(const either& other)   : is_left_{other.is_left_} {
        if (other.is_left_) {
            new (const_cast<L*>(&left_ .value))L{other.left_ .value}; 
        } else {
            new (const_cast<R*>(&right_.value))R{other.right_.value};
        }
    }

    public: ~either() {
        if (is_left_) {
            left_.value.~L();
        } else {
            right_.value.~R();
        }
    }

    public: template<class Identity> const auto fmap(const Identity& f) const {
        using E = either<L, decltype(f(right_.value))>;
        return is_left_ ? E{left{left_.value}} : E{right{f(right_.value)}};
    }

    public: template<class Applicative> const auto ap(const Applicative& a_x) const {
        using E = decltype(a_x.fmap(right_.value));
        return is_left_ ? E{left{left_.value}} : a_x.fmap(right_.value);
    }

    public: template<class F> const auto bind(const F& f) const {
        using E = decltype(f(right_.value));
        return is_left_ ? E{left{left_.value}} : f(right_.value);
    }

    public: template<class F> 
    const auto merge(const F& left_f) const {
        return [this, &left_f](const auto& right_f) {
            return is_left_ ? left_f(left_.value): right_f(right_.value);
        };
    }
};



const auto merge = [](const auto& left) {
    return [&left](const auto& right) {
        return [&left, &right](const auto& either) {
            return either.merge(left)(right);
        };
    };
};


}

/*
using namespace latte;

int main() {

    const auto lambda_id = id();

    const int x = 10;

    lambda_id >= either<int, id>{right{id()}} <= 10;

    return 0;
}
*/

#endif


















