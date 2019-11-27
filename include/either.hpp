
#ifndef LATTE_EITHER_H
#define LATTE_EITHER_H

#include <utility>

namespace latte {

template<class LT> struct left  { const LT value; };
template<class RT> struct right { const RT value; };

template<class L, class R>
class either {

    const bool is_left_;
    const union { const left<L> left_; const right<R> right_; };

    either(const left <L>& left) : is_left_{true},  left_ {left} {}
    either(const right<R>& right) : is_left_{false}, right_{right} {}

    public: static auto left (L value) { return either<L, R>{ latte::left <L>{value}}; }
    public: static auto right(R value) { return either<L, R>{ latte::right<R>{value}}; }

    public: ~either() {
        if (is_left_) {
            left_.value.~L();
        } else {
            right_.value.~R();
        }
    }

    public: const bool is_left () const { return  is_left_; }
    public: const bool is_right() const { return !is_left(); }

    public: either(const either& other) : is_left_{other.is_left()} {
        if (other.is_left()) {
            new (const_cast<L*>(&left_ .value))L{other.left_ .value}; 
        } else {
            new (const_cast<R*>(&right_.value))R{other.right_.value};
        }
    }

    public: template<class F> auto fmap(const F& f) const {
        using next_either = either<L, decltype(f(right_.value))>;
        return is_left() ? next_either::left(left_.value) : next_either::right(f(right_.value));
    }

    public: template<class F> auto bind(const F& f) const {
        using next_either = decltype(f(right_.value));
        return is_left() ? next_either::left(left_.value) : f(right_.value);
    }

    public: template<class LF, class RF> auto merge(const LF& left_f, const RF& right_f) const {
        return is_left() ? left_f(left_.value): right_f(right_.value);
    }

    public: auto reverse() const {
        using next_either = either<R, L>;
        return is_left() ? next_either::right(left_.value) : next_either::left(right_.value);
    }
};

}

#endif

