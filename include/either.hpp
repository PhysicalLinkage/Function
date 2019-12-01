
#ifndef LATTE_EITHER_H
#define LATTE_EITHER_H

#include <utility>

namespace latte {

template<class L, class R>
class either {

    template<class T> struct left_wrapper  { 
        const T value;
        left_wrapper(const T& value_) : value{value_} {}
    };
    
    template<class T> struct right_wrapper { 
        const T value;
        right_wrapper(const T& value_) : value{value_} {} 
    };

    const bool is_left_;
    const union { const L left_; const R right_; };

    either(const left_wrapper<L>&  wrapper) : is_left_{true},  left_ {wrapper.value} {}
    either(const right_wrapper<R>& wrapper) : is_left_{false}, right_{wrapper.value} {}
    either(const either& other)   : is_left_{other.is_left_} {
        if (other.is_left_) {
            new (const_cast<L*>(&left_))L{other.left_}; 
        } else {
            new (const_cast<R*>(&right_))R{other.right_};
        }
    }

    public: static const auto left (const L& value) { return either( left_wrapper<L>{value}); }
    public: static const auto right(const R& value) { return either(right_wrapper<R>{value}); }

    public: ~either() {
        if (is_left_) {
            left_.~L();
        } else {
            right_.~R();
        }
    }

    public: template<class Identity> const auto fmap(const Identity& f) const {
        using E = either<L, decltype(f(right_))>;
        return is_left_ ? E::left(left_) : E::right(f(right_));
    }

    public: template<class Applicative> const auto ap(const Applicative& a_x) const {
        using E = decltype(a_x.fmap(right_));
        return is_left_ ? E::left(left_) : a_x.fmap(right_);
    }

    public: template<class F> const auto bind(const F& f) const {
        using E = decltype(f(right_));
        return is_left_ ? E::left(left_) : f(right_);
    }

    public: template<class F> 
    const auto merge(const F& left_f) const {
        return [this, &left_f](const auto& right_f) {
            return is_left_ ? left_f(left_): right_f(right_);
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


















