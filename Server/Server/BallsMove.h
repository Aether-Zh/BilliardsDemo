//
//  BallsMove.h
//  Server
//
//  Created by duole_083 on 2020/7/20.
//  Copyright © 2020年 duole_083. All rights reserved.
//

#ifndef BallsMove_h
#define BallsMove_h

#include <vector>
#include <thread>
#include <mutex>

#include "Ball.h"

const int SUC = 1;
const int FAIL = 0;

const double EPS = 1e-8;

#define Epual(a,b) (fabs((a)-(b))<(EPS))
#define More(a,b) (((a)-(b))>(EPS))
#define Less(a,b) (((a)-(b))<(-EPS))
#define MoreEqu(a,b) (((a)-(b))>(-EPS))
#define LessEqu(a,b) (((a)-(b))<(EPS))

const int TPS = 30;                         // Trans rate per second. (1 / s)
const int FPS = 144;                        // Refresh rate per second. (1 / s)
const int CALC_T = 10000;                   // Calc rate per second. (um)

const double FRICT = 5e-3;

const int WIDTH = 3850;
const int HEIGHT = 2060;

const double BASKET_R = 42.5f;

static class BallsMove {
public:
    BallsMove() {
        scene_size_ = std::make_pair(WIDTH, HEIGHT);
        calc_run();
    }
    ~BallsMove() {
        for (auto i : pballs_) delete(i);
        pballs_.clear();
    }
    
    BallsMove(const BallsMove&) = delete;
    BallsMove(BallsMove&&) = delete;
    
    void add_ball(Ball* pb);
    void rm_ball(const int sock, const int index);
    std::vector<Ball*> get_balls();
private:
    bool in_world(const Ball* b);
    int crash(const Ball* b1, const Ball* b2);
    void calc_run();

    std::vector<Ball*> pballs_;
    std::pair<int, int> scene_size_;
    std::mutex mtx_;
} balls_world;

#endif /* BallsMove_h */
