//  Physical.hpp
//  Aether_Zhang

#ifndef PHYSICAL_H
#define PHYSICAL_H

#include <math.h>
#include "cocos2d.h"

namespace physical {
    // Calculate the distance between two points.
    inline double distance(const cocos2d::Point& p1, const cocos2d::Point& p2) {
        return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    }
    // Calculate vector's dot multiplication.
    inline double vec_dot_mult(const cocos2d::Vec2& p1, const cocos2d::Vec2& p2) {
        return 0.0f;
    }
    // Calculate vector's modular multiplication.
    inline double vec_modular_mult(const cocos2d::Vec2& p1, const cocos2d::Vec2& p2) {
        return 0.0f;
    }
    // Calculate vector's addition. (px / ms, rad)
    inline cocos2d::Vec2 vec_add(const cocos2d::Vec2& v1, const cocos2d::Vec2& v2) {
        double x = v1.x * cos(v1.y) + v2.x * cos(v2.y), y = v1.x * sin(v1.y) + v2.x * sin(v2.y);
        return cocos2d::Vec2(sqrt(pow(x, 2) + pow(y, 2)), atan2(y, x));
    }
};

#endif /* PHYSICAL_H */
