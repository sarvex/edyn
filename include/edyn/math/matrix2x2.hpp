#ifndef EDYN_MATH_MATRIX2X2_HPP
#define EDYN_MATH_MATRIX2X2_HPP

#include <array>
#include "edyn/config/config.h"
#include "edyn/math/vector2.hpp"

namespace edyn {

struct matrix2x2 {
    std::array<vector2, 2> row;

    vector2 & operator[](size_t i) {
        EDYN_ASSERT(i < 2);
        return row[i];
    }

    const vector2 & operator[](size_t i) const {
        EDYN_ASSERT(i < 2);
        return row[i];
    }

    inline vector2 column(size_t i) const {
        return {row[0][i], row[1][i]};
    }

    inline scalar column_dot(size_t i, const vector2 &v) const {
        return row[0][i] * v.x + row[1][i] * v.y;
    }

    inline scalar determinant() const {
        return perp_product(row[0], row[1]);
    }
};

// Identity matrix.
inline constexpr matrix2x2 matrix2x2_identity {{vector2_x, vector2_y}};

// Zero matrix.
inline constexpr matrix2x2 matrix2x2_zero {{vector2_zero, vector2_zero}};

// Add two matrices.
inline matrix2x2 operator+(const matrix2x2 &m, const matrix2x2 &n) {
    return {m.row[0] + n.row[0], m.row[1] + n.row[1]};
}

// Subtract two matrices.
inline matrix2x2 operator-(const matrix2x2 &m, const matrix2x2 &n) {
    return {m.row[0] - n.row[0], m.row[1] - n.row[1]};
}

// Multiply two matrices.
inline matrix2x2 operator*(const matrix2x2 &m, const matrix2x2 &n) {
    return {
        vector2{n.column_dot(0, m.row[0]), n.column_dot(1, m.row[0])},
        vector2{n.column_dot(0, m.row[1]), n.column_dot(1, m.row[1])}
    };
}

// Multiply vector by matrix.
inline vector2 operator*(const matrix2x2 &m, const vector2 &v) {
    return {dot(m.row[0], v), dot(m.row[1], v)};
}

// Multiply vector by matrix on the right, effectively multiplying
// by the transpose.
inline vector2 operator*(const vector2 &v, const matrix2x2 &m) {
    return {m.column_dot(0, v), m.column_dot(1, v)};
}

// Multiply matrix by scalar.
inline matrix2x2 operator*(const matrix2x2& m, scalar s) {
    return {m[0] * s, m[1] * s};
}

// Multiply scalar by matrix.
inline matrix2x2 operator*(scalar s, const matrix2x2& m) {
    return {s * m[0], s * m[1]};
}

// Add one matrix to another.
inline matrix2x2 & operator+=(matrix2x2 &m, const matrix2x2 &n) {
    m.row[0] += n.row[0];
    m.row[1] += n.row[1];
    return m;
}

// Subtract one matrix from another.
inline matrix2x2 operator-=(matrix2x2 &m, const matrix2x2 &n) {
    m.row[0] -= n.row[0];
    m.row[1] -= n.row[1];
    return m;
}

// Create a matrix with the given column vectors.
inline matrix2x2 matrix2x2_columns(const vector2 &v0,
                                   const vector2 &v1) {
    return {
        vector2{v0.x, v1.x},
        vector2{v0.y, v1.y}
    };
}

// Transpose of a 3x3 matrix.
inline matrix2x2 transpose(const matrix2x2 &m) {
    return {m.column(0), m.column(1)};
}

// Inverse of a 2x2 matrix or the zero matrix if `m` is non-invertible.
inline matrix2x2 inverse_matrix(const matrix2x2 &m) {
    auto det = m.determinant();
    scalar det_inv = 0;

    if (std::abs(det) > EDYN_EPSILON) {
        det_inv = scalar(1) / det;
    }

    return matrix2x2_columns({m.row[1].y, -m.row[1].x}, {-m.row[0].y, m.row[0].x}) * det_inv;
}

// Matrix with given vector as diagonal.
inline matrix2x2 diagonal_matrix(const vector2 &v) {
    return {
        vector2 {v.x, 0},
        vector2 {0, v.y}
    };
}

inline vector2 get_diagonal(const matrix2x2 &m) {
    return {m[0][0], m[1][1]};
}

// Equivalent to m * diagonal_matrix(v).
inline matrix2x2 scale_matrix(const matrix2x2 &m, const vector2 &v) {
    return {
        vector2{m.row[0].x * v.x, m.row[0].y * v.y},
        vector2{m.row[1].x * v.x, m.row[1].y * v.y}
    };
}

inline matrix2x2 rotation_matrix2x2(scalar angle) {
    auto cos = std::cos(angle);
    auto sin = std::sin(angle);

    return {
        vector2{cos, -sin},
        vector2{sin, cos}
    };
}

}

#endif // EDYN_MATH_MATRIX2X2_HPP
