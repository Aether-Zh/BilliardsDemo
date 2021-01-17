//  MoveBalls.h
//  Aether_Zhang

#ifndef MOVE_BALLS_H
#define MOVE_BALLS_H

#include <math.h>

#include <string>
#include <vector>
#include <condition_variable>
#include <mutex>

#include "cocos2d.h"
#include "Ball.h"
#include "ui/CocosGUI.h"

const int SUC = 1;
const int FAIL = 0;

const double ACCURACY = 0.01f;          // The accuracy error.
const int TPS = 20;                     // 
const int FPS = 60;                     // Refresh rate per second.     (1 / s)
const int CALC_T = 10000;               // Calc rate per second.        (um)

class MoveBalls : public cocos2d::Layer
{
public:
    // There's no 'id' in cpp, so we recommend returning the class instance pointer.
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone.
    virtual bool init();
    
    // Implement the "static create()" method manually.
    CREATE_FUNC(MoveBalls);
private:
    int crash(Ball* pb1, Ball* pb2);                                                                    // Check whether the two balls crashing.
    int crash(Ball* pb, const std::vector<cocos2d::Point>& vertex, const cocos2d::Point& center);       // Check whether the ball in scence.
    
    void init_touch_event();
    void init_button();
    
    bool onTouchBegan(cocos2d::Touch* tTouch, cocos2d::Event* eEvent);
    void onTouchMoved(cocos2d::Touch* tTouch, cocos2d::Event* eEvent);
    void onTouchEnded(cocos2d::Touch* tTouch, cocos2d::Event* eEvent);
    
    void calc_task(Ball* ball);
    void paint_task();
    void send_task();
    
    cocos2d::ui::Button* pbutton_ = nullptr;    // Clear button.
    Ball* pball_ = nullptr;                     // Currently ball's pointer.
    cocos2d::Size scene_size_;                  // Scene size includes width and height.
    std::vector<Ball*> balls_;                  // All balls.
//    std::pair<Ball*, Ball*> two_crash_;
    
    std::mutex mtx_;
//    std::condition_variable cv_;
//    bool wait = false;
};

#endif // MOVE_BALLS_H
