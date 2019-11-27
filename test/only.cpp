#include <either.hpp>

namespace latte {

template<std::size_t ID, class T>
struct distinguisher { const T value; };

template<std::size_t ID, class T1, class T2, class... Ts>
class only {
    using L = distinguisher<ID, T1>;
    using R = only<ID + 1, T2, Ts...>;
    using E = either<L, R>;

    E either_;
    
    public: template<class T>
    only(const std::size_t& id, const T& value) 
        : either_{(id == ID) ? 
            E{left {L{value}}}: 
            E{right{R{id, value}}}} {}
   
    public: template<class F>
    const auto fmap(const F& f) const {
        return either_.fmap([&f](const R& only_) { return only_.fmap(f); });
    }

    public: template<class F>
    const auto bind(const F& f) const {
        return either_.bind([&f](const R& only_) { return only_.bind(f); });
    };

    public: template<class F1, class F2, class... Fs>
    auto merge(const F1& left_f) const {
        return [&left_f](const F2& right_f) {
            return either_.merge(left_f
};

template<std::size_t ID, class T1, class T2>
class only<ID, T1, T2> {
    static constexpr std::size_t size_  = 2;
    using L = distinguisher<size_ - 1 , T1>;
    using R = T2;
    using E = either<L, R>;
   
    E either_;

    public: template<class T>
    only(std::size_t id, const T& value)
        : either_{(id == (size_ - 1)) ?
            E{left {L{value}}}:
            E{right{R{value}}}} {}

    public: template<class F> const auto fmap(const F& f) const {
        return either_.fmap([&f](const R& value) { return f(value); });
    }

    public: template<class F> const auto bind(const F& f) const {
        return either_.bind([&f](const R& value) { return f(value); });
    };


};


}

using namespace latte;

int main() {
    
	return 0;
}

