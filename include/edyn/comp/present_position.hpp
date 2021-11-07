#ifndef EDYN_COMP_PRESENT_POSITION_HPP
#define EDYN_COMP_PRESENT_POSITION_HPP

#include "edyn/math/vector2.hpp"

namespace edyn {

struct present_position : public vector2 {
    present_position & operator=(const vector2 &v) {
        vector2::operator=(v);
        return *this;
    }
};

}

#endif // EDYN_COMP_PRESENT_POSITION_HPP