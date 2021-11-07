#ifndef EDYN_COMP_AABB_HPP
#define EDYN_COMP_AABB_HPP

#include "edyn/math/vector2.hpp"
#include "edyn/math/geom.hpp"

namespace edyn {

/**
 * @brief Axis-aligned bounding box.
 */
struct AABB {
    vector2 min;
    vector2 max;

	inline AABB inset(const vector2 &v) const {
		return {min + v, max - v};
	}

    inline vector2 center() const {
        return (min + max) * scalar(0.5);
    }

    // Returns this AABB's perimer.
    inline scalar perimeter() const {
        return (max.x - min.x + max.y - min.y) * scalar(2);
    }

    // Returns whether this AABB contains point `p`.
    bool contains(const vector2 &p) const {
        return min <= p && p <= max;
    }

    // Returns whether `aabb` is contained within this AABB.
    bool contains(const AABB &aabb) const {
        return contains(aabb.min) && contains(aabb.max);
    }
};

inline bool intersect(const AABB &b0, const AABB &b1) {
    return intersect_aabb(b0.min, b0.max, b1.min, b1.max);
}

inline AABB enclosing_aabb(const AABB &b0, const AABB &b1) {
    return {
        min(b0.min, b1.min),
        max(b0.max, b1.max)
    };
}

}

#endif // EDYN_COMP_AABB_HPP
