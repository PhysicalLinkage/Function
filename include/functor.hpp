
#ifndef LATTE_FUNCTOR_H
#define LATTE_FUNCTOR_H

#include <either.hpp>

namespace latte {


// application
class class_identity_application {
    public: template<class Value>
    const auto operator()(const Value& x) const {
        return [&x](const auto& f) {
            return f(x);
        };
    }
};

const auto identity_application = class_identity_application();

template<class Identity, class Identity_>
const auto operator<(const Identity& f, const Identity_& x) {
    return identity_application(x)(f);
}



// composition
class class_identity_composition {
    public: template<class Identity>
    const auto operator()(const Identity& f) const {
        return [&f](const auto& g) {
            return [&f, &g](const auto& x) {
                return f(g(x));
            };
        };
    }
};

const auto identity_composition = class_identity_composition();

template<class Identity, class Identity_>
const auto operator>(const Identity& f, const Identity_& g) {
    return identity_composition(f)(g);
}




// functor
class class_functor_application {
    public: template<class Functor>
    const auto operator()(const Functor& f_a) const {
        return [&f_a](const auto& f) {
            return f_a.fmap(f);
        };
    }
};

const auto functor_application = class_functor_application();

template<class Identity, class Functor>
const auto operator<<(const Identity& f, const Functor& f_a) {
    return functor_application(f_a)(f);
}



class class_id {
    public: template<class Value>
    const auto operator()(const Value& x) const {
        return x;
    }
};

const auto id = class_id();

}

#include <stdio.h>

class class_log {
    public: template<class Value>
    const auto operator()(const Value& x) const {
        printf("log : %d\n", x);
        return x;
    }
};

const auto log = class_log();

#include <string>

using namespace latte;

const auto is_zero = [](const int& x) {
    using e = either<std::string, int>;
    return (x == 0) ?
        e{right{0}} :
        e{left{std::string{"ok"}}};
};
        


int main() {

    log(1);

    log < 2;

    log > log < 3;
    
    const auto e = either<std::string, int>{right{10}};

    log << e;

    return 0;
}

#endif









