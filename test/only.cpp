#include <either.hpp>

namespace latte {

template<std::size_t ID, class T>
struct distinguisher { const T value; };

template<class T1, class T2, class... Ts>
class only {
    static constexpr std::size_t size_ = sizeof...(Ts) + 2;
    using L = distinguisher<size_ - 1, T1>;
    using R = only<T2, Ts...>;
    using E = either<L, R>;

    E either_;
    
    public: static const std::size_t size(std::size_t offset = 0) {
        return R::size(offset + 1);
    }

    public: template<class T>
    only(std::size_t id, const T& value) 
        : either_{(id == (size_ - 1)) ? 
            E{left {L{value}}}: 
            E{right{R{id, value}}}} {}
   
    public: const std::size_t id(std::size_t offset = 0) const {
        return either_.merge([&offset](const L&) {
            return size() - offset + 1;
        }, [&offset](const R& right) {
            return right.id(offset + 1);
        });
    }
};

template<class T1, class T2>
class only<T1, T2> {
    static constexpr std::size_t size_  = 2;
    using L = distinguisher<size_ - 1 , T1>;
    using R = T2;
    using E = either<L, R>;
   
    E either_;

    public: static const std::size_t size(std::size_t offset = 0) {
        return offset + 2;
    }

    public: template<class T>
    only(std::size_t id, const T& value)
        : either_{(id == (size_ - 1)) ?
            E{left {L{value}}}:
            E{right{R{value}}}} {}

    public: const std::size_t id(std::size_t offset = 0) const {
        return either_.merge([&offset](const L&) {
            return size() - offset + 1;
        }, [&offset](const R&) {
            return size() - offset + 2;
        });
    }
            
};


}

using namespace latte;

int main() {
    
	return 0;
}

