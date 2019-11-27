#include <either.hpp>

namespace latte {

template<std::size_t ID, class T>
struct distinguisher { const T value; };

template<class T1, class T2, class... Ts>
class only {
    static constexpr std::size_t Ts_size = sizeof...(Ts);
    using L = distinguisher<Ts_size, T1>;
    using R = only<T2, Ts...>;
    using E = either<L, R>;

    E either_;

    only(const L& value) : either_{E:: left(value)} {}
    only(const R& value) : either_{E::right(value)} {}

    public: template<size_t ID, class T> 
    static auto of(const T& value) {
        return (ID == Ts_size) ?
            only{L    {value}}:
            only{R::of(value)};
    }


    
};

template<class T1, class T2, class T3>
class only<T1, T2, T3> {
    static constexpr std::size_t Ts_size = 1;
    using L = distinguisher<Ts_size, T1>;
    using R = either<L, T3>;
    using E = either<L, R>;
    
    E either_;

    only(const L& value) : either_{E:: left(value)} {}
    only(const R& value) : either_{E::right(value)} {}

    public: template<size_t ID, class T>
    static auto of(const T& value) {
        return (ID == Ts_size) ?
            only{distinguisher<ID, T>{value}}:
            only{
        
};


}

using namespace latte;

int main() {
    
	return 0;
}

