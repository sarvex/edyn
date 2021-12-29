#ifndef EDYN_SERIALIZATION_SHAE_CYLINDER_SHAPE_S11N_HPP
#define EDYN_SERIALIZATION_SHAE_CYLINDER_SHAPE_S11N_HPP

#include "edyn/shapes/cylinder_shape.hpp"

namespace edyn {

template<typename Archive>
void serialize(Archive &archive, cylinder_shape &s) {
    archive(s.half_length);
    archive(s.radius);
}

template<typename Archive>
void serialize(Archive &archive, cylinder_feature &f) {
    serialize_enum(archive, f);
}

}

#endif // EDYN_SERIALIZATION_SHAE_CYLINDER_SHAPE_S11N_HPP