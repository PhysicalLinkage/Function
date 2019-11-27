#ifndef LATTE_EITHER_H
#define LATTE_EITHER_H

#include <utility>

namespace Latte {

template<class L, class R>
class Either {
    template<class LT> struct Left_  { const LT value; };
    template<class RT> struct Right_ { const RT value; };

    const bool is_left_;
    const union { const Left_<L> left_; const Right_<R> right_; };

    const L& left () const { return left_ .value; }
    const R& right() const { return right_.value; }
    
    Either(const Left_ <L>& value) : is_left_{true},  left_ {value} {}
    Either(const Right_<R>& value) : is_left_{false}, right_{value} {}

    public: static auto Left (const L& value) { return Either<L, R>{Left_ <L>{value}}; }
    public: static auto Right(const R& value) { return Either<L, R>{Right_<R>{value}}; }

    public: ~Either() {
        if (is_left()) {
            left().~L();
        } else {
            right().~R();
        }
    }

    public: const bool is_left () const { return  is_left_; }
    public: const bool is_right() const { return !is_left(); }

    public: Either(const Either& other) : is_left_{other.is_left()} {
        if (other.is_left()) {
            new (const_cast<L*>(&left_ .value))L{other.left ()};
        } else {
            new (const_cast<R*>(&right_.value))R{other.right()};
        }
    }

    public: template<class F> auto fmap(const F& f) const {
        using NextEither = Either<L, decltype(f(right()))>;
        return is_left() ? NextEither::Left(left()) : NextEither::Right(f(right()));
    }

    public: template<class F> auto bind(const F& f) const {
        using NextEither = decltype(f(right()));
        return is_left() ? NextEither::Left(left()) : f(right());
    }

    public: template<class LF, class RF> auto merge(const LF& lf, const RF& rf) const {
        return is_left() ? lf(left()): rf(right());
    }

    public: auto reverse() const {
        using NextEither = Either<R, L>;
        return is_left() ? NextEither::Right(left()) : NextEither::Left(right());
    }
};

}

#endif

