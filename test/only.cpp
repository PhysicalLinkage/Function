#include <either.hpp>
#include <limits>

namespace latte {

namespace private_ {

template<class T, std::size_t ID = std::numeric_limits<std::size_t>::max()>
struct distinguisher { const T value; };

template<std::size_t ID, class T1, class T2, class... Ts>
class only_ {
    using L = distinguisher<T1, ID>;
    using R = only_<ID + 1, T2, Ts...>;
    using E = either<L, R>;

    E either_;
    
    public: template<class T>
    only_(const T& value, std::size_t id = std::numeric_limits<std::size_t>::max()) 
        : either_{(id == ID) ? 
            E{left {L{value}}}: 
            E{right{R{value, id}}}} {}
    public:
    only_(const only_& other) : either_{other.either_} {}
   
    public: template<class F>
    const auto fmap(const F& f) const {
        return either_.fmap([&f](const R& only_) { return only_.fmap(f); });
    }

    public: template<class F>
    const auto bind(const F& f) const {
        return either_.bind([&f](const R& only_) { return only_.bind(f); });
    };

    public: template<class F1, class F2, class... Fs>
    auto merge(const F1& f, const F2& next_f, const Fs&... fs) {
        const auto left_f  = [&f](const L& dist) { return f(dist.value); };
        const auto right_f = [&](const R& only_) { return only_.merge(next_f, fs...); };
        return either_.merge(left_f)(right_f);
    };

            /*
    public: template<class F>
    auto merge(const F& left_f) const {
        return [this, &left_f](const auto& next_f) {
            const auto right_f = [&next_f](const R& only_) { return only_.merge(next_f); };

        };
    }*/
};

template<std::size_t ID, class L, class R>
class only_<ID, L, R> {
    using E = either<L, R>;
   
    E either_;

    public: template<class T>
    only_(const T& value, std::size_t id = std::numeric_limits<std::size_t>::max())
        : either_{(id == ID) ?
            E{left {L{value}}}:
            E{right{value}}} {}
    public:
    only_(const only_& other) : either_{other.either_} {}

    public: template<class F> const auto fmap(const F& f) const {
        return either_.fmap([&f](const R& value) { return f(value); });
    }

    public: template<class F> const auto bind(const F& f) const {
        return either_.bind([&f](const R& value) { return f(value); });
    };

    public: template<class LF, class RF>
    auto merge(const LF& left_f, const RF& right_f) const {
        return either_.merge(left_f)(right_f);
    }
};

}

template<class T1, class T2, class... Ts>
using only = private_::only_<0, T1, T2, Ts...>;

}

#include <iostream>
#include <string>

using namespace latte;

int main() {
    
    const auto& range_filter = [](const auto& min) {
        using T = decltype(min);
        return [&min](const T& max) {
            return [&min, &max](const T& x) {
                using S = std::string;
                using O = only<S, S, S>;
                return  
                    (x < min) ? O{S{"min"}, 0} : 
                    (x > max) ? O{S{"max"}, 1} :
                    O{S{"x"}};
            };
        };
    };

    const auto& id = [](const auto& x) { return x; };

    std::cout << range_filter(0)(10)(-4).merge(id, id, id) << "\n";
    std::cout << range_filter(0)(10)(4).merge(id, id, id) << "\n";
    std::cout << range_filter(0)(10)(48).merge(id, id, id) << "\n";

	return 0;
}

