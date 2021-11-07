#ifndef EDYN_MATH_VECTOR2_HPP
#define EDYN_MATH_VECTOR2_HPP

#include <cmath>
#include "edyn/math/scalar.hpp"
#include "edyn/config/config.h"

namespace edyn {

struct vector2 {
    scalar x, y;

    scalar& operator[](size_t i) {
        EDYN_ASSERT(i < 2);
        return (&x)[i];
    }

    scalar operator[](size_t i) const {
        EDYN_ASSERT(i < 2);
        return (&x)[i];
    }
};

// Zero vector.
inline constexpr vector2 vector2_zero {0, 0};

// Vector with all elements set to 1.
inline constexpr vector2 vector2_one {1, 1};

// Unit vector pointing in the x direction.
inline constexpr vector2 vector2_x {1, 0};

// Unit vector pointing in the y direction.
inline constexpr vector2 vector2_y {0, 1};

// Vector with minumum values.
inline constexpr vector2 vector2_min {EDYN_SCALAR_MIN, EDYN_SCALAR_MIN};

// Vector with maximum values.
inline constexpr vector2 vector2_max {EDYN_SCALAR_MAX, EDYN_SCALAR_MAX};

// Add two vectors.
inline vector2 operator+(const vector2 &v, const vector2 &w) {
    return {v.x + w.x, v.y + w.y};
}

// Add a vector into another vector.
inline vector2& operator+=(vector2 &v, const vector2 &w) {
    v.x += w.x;
    v.y += w.y;
    return v;
}

// Subtract two vectors.
inline vector2 operator-(const vector2 &v, const vector2 &w) {
    return {v.x - w.x, v.y - w.y};
}

// Subtract a vector from another vector.
inline vector2& operator-=(vector2 &v, const vector2 &w) {
    v.x -= w.x;
    v.y -= w.y;
    return v;
}

// Negation of a vector.
inline vector2 operator-(const vector2 &v) {
    return {-v.x, -v.y};
}

// Multiply vectors component-wise.
inline vector2 operator*(const vector2 &v, const vector2 &w) {
    return {v.x * w.x, v.y * w.y};
}

// Multiply vector by scalar.
inline constexpr vector2 operator*(const vector2& v, scalar s) {
    return {v.x * s, v.y * s};
}

// Multiply scalar by vector.
inline vector2 operator*(scalar s, const vector2 &v) {
    return {s * v.x, s * v.y};
}

// Divide vector by scalar.
inline vector2 operator/(const vector2 &v, scalar s) {
    return {v.x / s, v.y / s};
}

// Divide scalar by vector.
inline vector2 operator/(scalar s, const vector2 &v) {
    return {s / v.x, s / v.y};
}

// Scale a vector.
inline vector2& operator*=(vector2 &v, scalar s) {
    v.x *= s;
    v.y *= s;
    return v;
}

// Inverse-scale a vector.
inline vector2& operator/=(vector2 &v, scalar s) {
    auto z = scalar(1) / s;
    v.x *= z;
    v.y *= z;
    return v;
}

// Multiply vectors component-wise and assign to the first.
inline vector2 & operator*=(vector2 &v, const vector2 &w) {
    v.x *= w.x;
    v.y *= w.y;
    return v;
}

// Check if two vectors are equal.
inline bool operator==(const vector2 &v, const vector2 &w) {
    return v.x == w.x && v.y == w.y;
}

// Check if two vectors are different.
inline bool operator!=(const vector2 &v, const vector2 &w) {
    return v.x != w.x || v.y != w.y;
}

// Check if a vector is bigger than another component-wise.
inline bool operator>(const vector2 &v, const vector2 &w) {
    return v.x > w.x && v.y > w.y;
}

// Check if a vector is smaller than another component-wise.
inline bool operator<(const vector2 &v, const vector2 &w) {
    return v.x < w.x && v.y < w.y;
}

// Check if a vector is greater than or equal to another component-wise.
inline bool operator>=(const vector2 &v, const vector2 &w) {
    return v.x >= w.x && v.y >= w.y;
}

// Check if a vector is less than or equal to another component-wise.
inline bool operator<=(const vector2 &v, const vector2 &w) {
    return v.x <= w.x && v.y <= w.y;
}

// Dot product between vectors.
inline scalar dot(const vector2 &v, const vector2 &w) {
    return v.x * w.x + v.y * w.y;
}

// Dot product between a vector and a perpendicular to the other vector.
inline scalar perp_product(const vector2 &v, const vector2 &w) {
    return v.x * w.y - v.y * w.x;
}

// Vector orthogonal to argument, i.e. rotated 90 degrees counter-clockwise.
// Negate result for a clockwise 90 degree rotation.
inline vector2 orthogonal(const vector2 &v) {
    return {-v.y, v.x};
}

// Square length of a vector.
inline scalar length_sqr(const vector2 &v) {
    return dot(v, v);
}

// Length of a vector.
inline scalar length(const vector2 &v) {
    return std::sqrt(length_sqr(v));
}

// Distance between two points.
inline scalar distance(const vector2 &p0, const vector2 &p1) {
    return length(p0 - p1);
}

// Squared distance between two points.
inline scalar distance_sqr(const vector2 &p0, const vector2 &p1) {
    return length_sqr(p0 - p1);
}

// Normalized vector (unit length). Asserts if the vector's length is zero.
inline vector2 normalize(const vector2 &v) {
    auto l = length(v);
    EDYN_ASSERT(l > EDYN_EPSILON);
    return v / l;
}

// Normalizes vector if it's length is greater than a threshold above zero.
// Returns where the vector was normalized.
inline bool try_normalize(vector2 &v) {
    auto lsqr = length_sqr(v);

    if (lsqr > EDYN_EPSILON) {
        v /= std::sqrt(lsqr);
        return true;
    }

    return false;
}

// Projects direction vector `v` onto plane with normal `n`.
inline vector2 project_direction(const vector2 &v, const vector2 &n) {
    return v - n * dot(v, n);
}

// Projects point `p` onto plane with origin `q` and normal `n`.
inline vector2 project_plane(const vector2 &p, const vector2 &q, const vector2 &n) {
    return p - n * dot(p - q, n);
}

// Performs element-wise minimum.
inline vector2 min(const vector2 &v, const vector2 &w) {
    return {std::min(v.x, w.x), std::min(v.y, w.y)};
}

// Performs element-wise maximum.
inline vector2 max(const vector2 &v, const vector2 &w) {
    return {std::max(v.x, w.x), std::max(v.y, w.y)};
}

// Performs element-wise absolute.
inline vector2 abs(const vector2 &v) {
    return {std::abs(v.x), std::abs(v.y)};
}

// Returns the index of the coordinate with greatest value.
inline size_t max_index(const vector2 &v) {
    return v.x > v.y ? 0 : 1;
}

// Returns the index of the coordinate with greatest absolute value.
inline size_t max_index_abs(const vector2 &v) {
    return max_index(abs(v));
}

}

#endif // EDYN_MATH_VECTOR2_HPP
