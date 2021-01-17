//  MoveBalls.h
//  Aether_Zhang

#ifndef MOVE_BALLS_H
#define MOVE_BALLS_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "workqueue.h"

#include <string>
#include <vector>
#include <map>

#include "Ball.h"

const int SUC = 1;
const int FAIL = 0;

const int WIDTH = 3850;
const int HEIGHT = 2060;

const double ACCURACY = 0.01f;          // The accuracy error.
const int TPS = 60;                     // Trans rate per second. (1 / s)
const int FPS = 144;                    // Refresh rate per second. (1 / s)
const int CALC_T = 10000;               // Calc rate per second. (um)


class MoveBalls : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(MoveBalls);
private:
    bool onTouchBegan(cocos2d::Touch* tTouch, cocos2d::Event* eEvent);
    void onTouchMoved(cocos2d::Touch* tTouch, cocos2d::Event* eEvent);
    void onTouchEnded(cocos2d::Touch* tTouch, cocos2d::Event* eEvent);
    
    void show_ball(void* msg_ptr);
    void rm_ball(void* msg_ptr);
    
    void recv_task();
    
//    std::map<std::string, Ball*> balls_;
    Ball* cur_ball_p_ = nullptr;
    CWorkQueue work_queue_;
    double scale_;                  // Scene size includes width and height.
    std::pair<double, double> offset_;
    int id_ = 0;
};

#endif // MOVE_BALLS_H
