#ifndef EDYN_COMP_INERTIA_HPP
#define EDYN_COMP_INERTIA_HPP

#include "edyn/comp/scalar_comp.hpp"

namespace edyn {

struct inertia : public scalar_comp {
    inertia & operator=(scalar t) {
        s = t;
        return *this;
    }
};

struct inertia_inv : public scalar_comp {
    inertia_inv & operator=(scalar t) {
        s = t;
        return *this;
    }
};

}

#endif // EDYN_COMP_INERTIA_HPP
