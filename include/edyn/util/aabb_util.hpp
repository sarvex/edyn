#ifndef EDYN_UTIL_AABB_UTIL_HPP
#define EDYN_UTIL_AABB_UTIL_HPP

#include "edyn/comp/aabb.hpp"
#include "edyn/shapes/shapes.hpp"

namespace edyn {

AABB box_aabb(const vector2 &half_extents, const vector2 &pos, scalar orn);
AABB circle_aabb(scalar radius, const vector2 &pos);
AABB circle_aabb(scalar radius, const vector2 &pos, scalar orn);
AABB capsule_aabb(scalar radius, scalar half_length, const vector2 &pos, scalar orn);

/**
 * @brief Calculates the AABB of an AABB transformed to world space.
 * @param aabb The AABB.
 * @param pos Position of AABB.
 * @param orn Orientation of AABB.
 * @return AABB of the given AABB with transformation applied.
 */
AABB aabb_to_world_space(const AABB &aabb, const vector2 &pos, scalar orn);

/**
 * @brief Calculates the AABB of an AABB transformed into object space.
 * @param aabb The AABB.
 * @param pos Center of object.
 * @param orn Orientation of object.
 * @return AABB of the given AABB in the space of another object.
 */
AABB aabb_to_object_space(const AABB &aabb, const vector2 &pos, scalar orn);

/**
 * @brief Calculates the AABB of a set of points.
 * @param points A point cloud.
 * @return AABB of point set.
 */
AABB point_cloud_aabb(const std::vector<vector2> &points);

/**
 * @brief Calculates the AABB of a set of points with a transformation.
 * @param points A point cloud.
 * @param pos Position offset applied to all points.
 * @param orn Orientation of point cloud.
 * @return AABB of point set.
 */
AABB point_cloud_aabb(const std::vector<vector2> &points,
                      const vector2 &pos, scalar orn);

// Calculate AABB for all types of shapes.
AABB shape_aabb(const plane_shape &sh, const vector2 &pos, scalar orn);
AABB shape_aabb(const sphere_shape &sh, const vector2 &pos, scalar orn);
AABB shape_aabb(const cylinder_shape &sh, const vector2 &pos, scalar orn);
AABB shape_aabb(const capsule_shape &sh, const vector2 &pos, scalar orn);
AABB shape_aabb(const mesh_shape &sh, const vector2 &pos, scalar orn);
AABB shape_aabb(const box_shape &sh, const vector2 &pos, scalar orn);
AABB shape_aabb(const polyhedron_shape &sh, const vector2 &pos, scalar orn);
AABB shape_aabb(const paged_mesh_shape &sh, const vector2 &pos, scalar orn);
AABB shape_aabb(const compound_shape &sh, const vector2 &pos, scalar orn);

/**
 * @brief Visits the shape variant and calculates the the AABB.
 * @param var The shape variant.
 * @param pos Shape's origin.
 * @param orn Shape's orientation.
 * @return The AABB.
 */
AABB shape_aabb(const shapes_variant_t &var, const vector2 &pos, scalar orn);

}

#endif // EDYN_UTIL_AABB_UTIL_HPP
