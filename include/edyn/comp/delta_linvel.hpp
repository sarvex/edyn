#ifndef EDYN_COMP_DELTA_LINVEL_HPP
#define EDYN_COMP_DELTA_LINVEL_HPP

#include "edyn/math/vector2.hpp"

namespace edyn {

struct delta_linvel : public vector2 {
    delta_linvel& operator=(const vector2& v) {
        vector2::operator=(v);
        return *this;
    }
};

}

#endif // EDYN_COMP_DELTA_LINVEL_HPP