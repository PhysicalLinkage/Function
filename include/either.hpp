
#ifndef LATTE_EITHER_H
#define LATTE_EITHER_H

#include <utility>

namespace latte {

template<class L, class R>
class either {

    template<class T> struct left_wrapper  { 
        const T value;
        constexpr explicit left_wrapper(const T& value_) noexcept: value{value_} {}
    };
    
    template<class T> struct right_wrapper { 
        const T value;
        constexpr explicit right_wrapper(const T& value_) noexcept : value{value_} {} 
    };

    const bool is_left_;
    const union { const L left_; const R right_; };

    constexpr explicit either(const left_wrapper<L>&  wrapper) noexcept : is_left_{true},  left_ {wrapper.value} {}
    constexpr explicit either(const right_wrapper<R>& wrapper) noexcept : is_left_{false}, right_{wrapper.value} {}
    constexpr explicit either(const either& other) noexcept : is_left_{other.is_left_} {
        if (other.is_left_) {
            new (const_cast<L*>(&left_))L{other.left_}; 
        } else {
            new (const_cast<R*>(&right_))R{other.right_};
        }
    }

    public: static constexpr const auto left (const L& value) noexcept { return either( left_wrapper<L>{value}); }
    public: static constexpr const auto right(const R& value) noexcept { return either(right_wrapper<R>{value}); }

    public: ~either() noexcept {
        if (is_left_) {
            left_.~L();
        } else {
            right_.~R();
        }
    }

    public: template<class Identity> constexpr const auto fmap(const Identity& f) const noexcept {
        using E = either<L, decltype(f(right_))>;
        return is_left_ ? E::left(left_) : E::right(f(right_));
    }

    public: template<class Applicative> constexpr const auto ap(const Applicative& a_x) const noexcept {
        using E = decltype(a_x.fmap(right_));
        return is_left_ ? E::left(left_) : a_x.fmap(right_);
    }

    public: template<class F> constexpr const auto bind(const F& f) const noexcept {
        using E = decltype(f(right_));
        return is_left_ ? E::left(left_) : f(right_);
    }

    public: template<class F>  constexpr const auto merge(const F& left_f) const noexcept {
        return [this, &left_f](const auto& right_f) constexpr noexcept {
            return is_left_ ? left_f(left_): right_f(right_);
        };
    }

};

constexpr const class merge {
    public: template<class T> constexpr const auto operator()(const T& left) const noexcept {
        return [&left](const auto& right) constexpr noexcept {
            return [&left, &right](const auto& either) constexpr noexcept {
                either.merge(left)(right);
            };
        };
    }
} merge;

}

#endif

