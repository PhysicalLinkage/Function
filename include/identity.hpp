
#ifndef LATTE_IDENTITY_H
#define LATTE_IDENTITY_H

namespace latte {

    template<class Identity1, class Identity2>
    const auto operator<(const Identity1& lambda, const Identity2& x) {
        return lambda(x);
    }

    template<class Identity1, class Identity2>
    const auto operator>(const Identity1& lambda1, const Identity2& lambda2) {
        return [&lambda1, &lambda2](const auto& x) {
            return lambda1(lambda2(x));
        };
    }

}

#endif

