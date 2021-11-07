#ifndef EDYN_COMP_GRAVITY_HPP
#define EDYN_COMP_GRAVITY_HPP

#include "edyn/math/vector2.hpp"

namespace edyn {

struct gravity : public vector2 {
    gravity & operator=(const vector2 &v) {
        vector2::operator=(v);
        return *this;
    }
};

}

#endif // EDYN_COMP_GRAVITY_HPP