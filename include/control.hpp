
#ifndef LATTE_CONTROL_H
#define LATTE_CONTROL_H

namespace latte {

namespace control {

constexpr const class functionalization {
    public: template<class Value>
    constexpr const auto operator()(const Value& x) const noexcept {
        return [&x](const auto& f) constexpr noexcept -> const auto {
            return f(x);
        };
    }
} functionalization;

template<class Identity, class Identity_>
constexpr const auto operator+(const Identity& f, const Identity_& x) noexcept {
    return functionalization(x)(f);
}

constexpr const class function_composition {
    public: template<class Identity>
    constexpr const auto operator()(const Identity& f) const noexcept {
        return [&f](const auto& g) constexpr noexcept -> const auto {
            return [&f, &g](const auto& x) constexpr noexcept -> const auto {
                return f(g(x));
            };
        };
    }
} function_composition;

template<class Identity, class Identity_>
constexpr const auto operator*(const Identity& f, const Identity_& g) noexcept {
    return function_composition(f)(g);
}

constexpr const class fmap {
    public: template<class Functor>
    constexpr const auto operator()(const Functor& f_x) const noexcept {
        return [&f_x](const auto& f) constexpr noexcept -> const auto {
            return f_x.fmap(f);
        };  
    }   
} fmap;

template<class Identity, class Functor>
constexpr const auto operator<(const Identity& f, const Functor& f_x) noexcept {
    return fmap(f_x)(f);
}

constexpr const class ap {
    public: template<class Applicative>
    constexpr const auto operator()(const Applicative& a_f) const noexcept {
        return [&a_f](const auto& a_x) constexpr noexcept -> const auto{
            return a_f.ap(a_x);
        };
    }
} ap;

template<class Applicative, class Applicative_>
constexpr const auto operator<=(const Applicative& a_f, const Applicative_& a_x) noexcept {
    return ap(a_f)(a_x);
}

constexpr const class bind {
    public: template<class Identity>
    constexpr const auto operator()(const Identity& f) const noexcept {
        return [&f](const auto& m_x) constexpr noexcept -> const auto {
            return m_x.bind(f);
        };
    }
} bind;

template<class Identity, class Monad>
constexpr const auto operator<<=(const Identity& f, const Monad& m_x) noexcept {
    return bind(f)(m_x);
};


} // namespace control

} // namespace latte

#endif // LATTE_CONTROL_H

