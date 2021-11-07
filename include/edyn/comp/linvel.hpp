#ifndef EDYN_COMP_VELOCITY_HPP
#define EDYN_COMP_VELOCITY_HPP

#include "edyn/math/vector2.hpp"

namespace edyn {

/**
 * @brief Linear velocity component.
 */
struct linvel : public vector2 {
    linvel & operator=(const vector2 &v) {
        vector2::operator=(v);
        return *this;
    }
};

}

#endif // EDYN_COMP_VELOCITY_HPP