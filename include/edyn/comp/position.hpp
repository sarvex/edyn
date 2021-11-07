#ifndef EDYN_COMP_POSITION_HPP
#define EDYN_COMP_POSITION_HPP

#include "edyn/math/vector2.hpp"

namespace edyn {

struct position : public vector2 {
    position & operator=(const vector2 &v) {
        vector2::operator=(v);
        return *this;
    }
};

}

#endif // EDYN_COMP_POSITION_HPP