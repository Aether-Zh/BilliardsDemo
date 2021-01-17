//  Physical.hpp
//  Aether_Zhang

#ifndef PHYSICAL_H
#define PHYSICAL_H

#include <math.h>

const double PI = 3.1415926f;           // PI ratio.                    (radian)
const int FLAT_ANGLE = 180;         // Flat angle.                  (angle)

namespace physical {
    // Calculate the distance between two points.
    inline double distance(const double& x1, const double& y1,
                           const double& x2, const double& y2) {
        return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    }
    // Calculate vector's dot multiplication.
    inline double vec_dot_mult(const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
        return 0.0f;
    }
    // Calculate vector's modular multiplication.
    inline double vec_modular_mult(const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
        return 0.0f;
    }
    // Calculate vector's addition. (px / ms, rad)
    inline std::pair<double, double> velocity_synthesis(const double& v1, const double& rad_1,
                                                        const double& v2, const double& rad_2) {
        const double& x = v1 * cos(rad_1) + v2 * cos(rad_2);
        const double& y = v1 * sin(rad_1) + v2 * sin(rad_2);
        return std::make_pair(sqrt(pow(x, 2) + pow(y, 2)), atan2(y, x));
    }
};

#endif /* PHYSICAL_H */
