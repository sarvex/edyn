#ifndef EDYN_MATH_CONSTANTS_HPP
#define EDYN_MATH_CONSTANTS_HPP

#include <cmath>
#include <float.h>
#include "vector2.hpp"

namespace edyn {

inline constexpr auto pi = scalar(3.1415926535897932384626433832795029);
inline constexpr auto pi2 = pi * scalar(2);
inline constexpr auto half_pi = pi * scalar(0.5);
inline constexpr auto half_sqrt2 = scalar(0.7071067811865475244008443621048490);
// `large_scalar * large_scalar` < EDYN_SCALAR_MAX`
inline constexpr auto large_scalar = scalar(1e18);
// `small_scalar * small_scalar` > EDYN_SCALAR_MIN`
inline constexpr auto small_scalar = scalar(1e-18);
inline constexpr auto gravitational_constant = scalar(6.674e-11);

inline constexpr vector2 gravity_sun     = vector2_y * scalar(-274);
inline constexpr vector2 gravity_mercury = vector2_y * scalar(-3.7);
inline constexpr vector2 gravity_venus   = vector2_y * scalar(-8.87);
inline constexpr vector2 gravity_earth   = vector2_y * scalar(-9.8);
inline constexpr vector2 gravity_mars    = vector2_y * scalar(-3.721);
inline constexpr vector2 gravity_jupiter = vector2_y * scalar(-24.79);
inline constexpr vector2 gravity_saturn  = vector2_y * scalar(-10.44);
inline constexpr vector2 gravity_uranus  = vector2_y * scalar(-8.69);
inline constexpr vector2 gravity_neptune = vector2_y * scalar(-11.15);
inline constexpr vector2 gravity_pluto   = vector2_y * scalar(-0.62);
inline constexpr vector2 gravity_moon    = vector2_y * scalar(-1.625);

}

#endif // EDYN_MATH_CONSTANTS_HPP
