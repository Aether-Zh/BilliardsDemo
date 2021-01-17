#ifndef BALL_H
#define BALL_H

#include <time.h>

#include <vector>
#include <random>

#include "Physical.h"

const double BALL_D = 52.5f;

const double MAX_V = 10.0f;               // Maximum speed.   (px / ms)
const double MIN_V = 1.0f;                // Minimum speed.   (px / ms)

struct Ball {
    Ball(int sock, int index, double x, double y) : sock_(sock), id_(index), r_(BALL_D / 2.0f), x_(x), y_(y), stratum_(0) {
        std::default_random_engine random(static_cast<unsigned int>(time(NULL)));
        std::uniform_int_distribution<int> rd_v_dir(1, 2 * FLAT_ANGLE);
        std::uniform_real_distribution<double> rd_v_val(MIN_V, MAX_V);
        v_ = std::make_pair(rd_v_val(random), rd_v_dir(random) * PI / FLAT_ANGLE);
    }
    
    int sock_, id_;
    double r_, x_, y_;                     // Base data.
    std::pair<double, double> v_;
    int stratum_;
};

#endif /* BALL_H */
