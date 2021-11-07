#ifndef EDYN_MATH_TRANSFORM_HPP
#define EDYN_MATH_TRANSFORM_HPP

#include "edyn/math/vector2.hpp"
#include "edyn/math/matrix2x2.hpp"

namespace edyn {

/**
 * @brief Converts a point in world space to object space.
 * @param p A point in world space.
 * @param pos Position in world space.
 * @param basis Rotation matrix in world space.
 * @return The point `p` in object space.
 */
inline
vector2 to_object_space(const vector2 &p, const vector2 &pos, const matrix2x2 &basis) {
    // Multiplying a vector by a matrix on the right is equivalent to multiplying
    // by the transpose of the matrix on the left, and the transpose of a rotation
    // matrix is its inverse.
    return (p - pos) * basis;
}

/**
 * @brief Converts a point in object space to world space.
 * @param p A point in object space.
 * @param pos Position in world space.
 * @param basis Rotation matrix in world space.
 * @return The point `p` in world space.
 */
inline
vector2 to_world_space(const vector2 &p, const vector2 &pos, const matrix2x2 &basis) {
    return pos + basis * p;
}

inline
vector2 to_object_space(const vector2 &p, const vector2 &pos, scalar orn) {
    return to_object_space(p, pos, rotation_matrix2x2(orn));
}

inline
vector2 to_world_space(const vector2 &p, const vector2 &pos, scalar orn) {
    return to_world_space(p, pos, rotation_matrix2x2(orn));
}

}

#endif // EDYN_MATH_TRANSFORM_HPP
