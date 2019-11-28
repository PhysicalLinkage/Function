#include <either.hpp>

namespace latte {

namespace private_ {

template<class F>
const auto merge_(const F& left_f) {
    return [&left_f](const auto& right_f) {
        return [&left_f, &right_f](const auto& either_) { 
            return (either_.merge(left_f)(right_f)); 
        };
    };
}

}

template<class F>
const auto merge(const F& left_f) { return private_::merge_(left_f); }

template<class F1, class F2, class F3, class... Fs>
class merges {

    const F1& f1_;
    
    public: merges(const F1& f1) : f1_{f1} {}

    const auto operator()(const F2& f2) {
        return [this, &f2](const F3& f3) {
            return merge(f1_)(merge<F2, F3, Fs...>(f2)(f3));
        };
    }
};

template<class F1, class F2, class F3>
class merges<F1, F2, F3> {

    const F1 f1_;

    public: merges(const F1& f1) : f1_{f1} {}

    const auto operator()(const F2& f2) {
        return [this, &f2](const F3& f3) {
            return merge(f1_)(merge(f2)(f3));
        };
    };

};

}

#include <iostream>
#include <string>

using namespace latte;

int main() {

    using S = std::string;
    using E1 = either<S, int>;
    using E = either<S, E1>;

    const auto range_filter = [](const auto& min) {
        using T = decltype(min);
        return [&min](const T& max) { 
            return [&min, &max](const T& x) {
                return  (x < min) ? E{left {S {"min"}}} :
                        (x > max) ? E{right{E1{left{S{"max"}}}}} :
                                    E{right{E1{right{x}}}};
            };
        };
    };

    const auto id = [](const auto& x) { return x; };
    const auto to_string = [](const auto& x) { return std::to_string(x); };

    const auto O_10 = range_filter(0)(10);

    using ID        = decltype(id);
    using TO_STRING = decltype(to_string);
    using Merges    = merges<ID, ID, TO_STRING>;

    std::cout << Merges(id)(id)(to_string)((O_10)(-5)) << "\n";
    std::cout << Merges(id)(id)(to_string)((O_10)( 5)) << "\n";
    std::cout << Merges(id)(id)(to_string)((O_10)(15)) << "\n";

    //x.merge(id)(merge(id)(merge(id)(merge(id)(id))));

    return 0;
}

