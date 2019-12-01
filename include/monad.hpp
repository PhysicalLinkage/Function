
#ifndef LATTE_MONAD_H
#define LATTE_MONAD_H

namespace latte {

template<class Return, class Monad>
const auto operator<<=(const Return& return_, const Monad& m_x) {
    return m_x.bind(return_);
}

}

#endif

