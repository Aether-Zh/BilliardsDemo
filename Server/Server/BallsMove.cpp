//
//  BallsMove.cpp
//  Server
//
//  Created by duole_083 on 2020/7/20.
//  Copyright © 2020年 duole_083. All rights reserved.
//

#include <thread>
#include <chrono>

#include "BallsMove.h"
#include "Physical.h"

int BallsMove::crash(const Ball* b1, const Ball* b2) {
    double dist = physical::distance(b1->x_, b1->y_, b2->x_, b2->y_);
    if (More(dist, 0) && LessEqu(dist, b1->r_ + b2->r_)) return SUC;
    return FAIL;
}
bool BallsMove::in_world(const Ball* b) {
    if (More(b->x_ - b->r_, 0) && Less(b->x_ + b->r_, scene_size_.first) &&
        More(b->y_ - b->r_, 0) && Less(b->y_ + b->r_, scene_size_.second))
        return true;
    return false;
}
void BallsMove::add_ball(Ball* pb) {
    if (in_world(pb)) {
        for (int i = 0; i < pballs_.size(); ++i) {
            if (crash(pb, pballs_[i])) return;
        }
        mtx_.lock();
        pballs_.emplace_back(pb);
        mtx_.unlock();
    }
}
void BallsMove::rm_ball(const int sock, const int index) {
    for (int i = 0; i < pballs_.size(); ++i) {
        if (pballs_[i]->sock_ == sock && pballs_[i]->id_ == index) {
            mtx_.lock();
            delete(pballs_[i]);
            pballs_.erase(pballs_.begin() + i--);
            mtx_.unlock();
        }
    }
}
std::vector<Ball*> BallsMove::get_balls() {
    mtx_.lock();
    std::vector<Ball*> ret = pballs_;
    mtx_.unlock();
    return ret;
}
void BallsMove::calc_run() {
    std::thread([&](){
        double l, r, t, b;
        double i_k, j_k, i_x_t, i_y_t, j_x_t, j_y_t;
        double t_v;
        std::pair<double, double> tmp_v;
        
        for (;;) {
            for (int i = 0; i < pballs_.size(); ++i) {
                l = b = pballs_[i]->r_;
                r = scene_size_.first - pballs_[i]->r_;
                t = scene_size_.second - pballs_[i]->r_;
                
                // left
                if (LessEqu(pballs_[i]->x_, l)) {
                    pballs_[i]->v_.second = -pballs_[i]->v_.second + PI;
                    while (LessEqu(pballs_[i]->x_, l)) ++pballs_[i]->x_;
                }
                // right
                if (MoreEqu(pballs_[i]->x_, r)) {
                    pballs_[i]->v_.second = -pballs_[i]->v_.second + PI;
                    while (MoreEqu(pballs_[i]->x_, r)) --pballs_[i]->x_;
                }
                // top
                if (MoreEqu(pballs_[i]->y_, t)) {
                    pballs_[i]->v_.second = -pballs_[i]->v_.second;
                    while (MoreEqu(pballs_[i]->y_, t)) --pballs_[i]->y_;
                }
                // bottom
                if (LessEqu(pballs_[i]->y_, b)) {
                    pballs_[i]->v_.second = -pballs_[i]->v_.second;
                    while (LessEqu(pballs_[i]->y_, b)) ++pballs_[i]->y_;
                }
            }
            
            // Balls crash check.
            for (int i = 0; i < static_cast<int>(pballs_.size()) - 1; ++i) {
                for (int j = i + 1; j < static_cast<int>(pballs_.size()); ++j) {
                    if (crash(pballs_[i], pballs_[j]) == SUC) {
                        i_k = atan2(pballs_[i]->y_ - pballs_[j]->y_, pballs_[i]->x_ - pballs_[j]->x_);
                        j_k = atan2(pballs_[j]->y_ - pballs_[i]->y_, pballs_[j]->x_ - pballs_[i]->x_);

                        // move out from the crashing
                        i_x_t = cos(i_k); i_y_t = sin(i_k); j_x_t = cos(j_k); j_y_t = sin(j_k);
                        do {
                            pballs_[i]->x_ += i_x_t; pballs_[i]->y_ += i_y_t;
                            pballs_[j]->x_ += j_x_t; pballs_[j]->y_ += j_y_t;
                        }while (in_world(pballs_[i]) && in_world(pballs_[j]) && crash(pballs_[i], pballs_[j]) == SUC);

                        // calc v
                        tmp_v = pballs_[i]->v_;
                        pballs_[i]->v_ = physical::velocity_synthesis(pballs_[i]->v_.first * sin(j_k - pballs_[i]->v_.second), i_k + PI / 2.0f,
                                                                      pballs_[j]->v_.first * cos(i_k - pballs_[j]->v_.second), i_k);
                        pballs_[j]->v_ = physical::velocity_synthesis(pballs_[j]->v_.first * sin(i_k - pballs_[j]->v_.second), j_k + PI / 2.0f,
                                                                      tmp_v.first * cos(j_k - tmp_v.second), j_k);
                    }
                }
            }
            
            // Move a distance.
            for (int i = 0; i < pballs_.size(); ++i) {
                pballs_[i]->x_ += pballs_[i]->v_.first * cos(pballs_[i]->v_.second);
                pballs_[i]->y_ += pballs_[i]->v_.first * sin(pballs_[i]->v_.second);
                
                
                if (More(pballs_[i]->v_.first, 0)) {
                    t_v = pballs_[i]->v_.first - FRICT;
                    if (More(t_v, 0)) {
                        pballs_[i]->v_.first = t_v;
                    }else {
                        pballs_[i]->v_.first = 0.0f;
                    }
                }
            }
            
            std::this_thread::sleep_for(std::chrono::microseconds(CALC_T));
        }
    }).detach();
}
