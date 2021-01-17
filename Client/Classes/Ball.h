//  Ball.h
//  Aether_Zhang

#ifndef BALL_H
#define BALL_H

#include "cocos2d.h"

class Ball : public cocos2d::Sprite {
public:
//    virtual bool init();
    bool init() {
        Sprite::setVisible(false);
        Sprite::initWithFile("ass/ball.png");   // Use a picture to represent the ball.
        return true;
    }
    ~Ball() = default;
    CREATE_FUNC(Ball);

    double r_, x_, y_;
};



#endif /* BALL_H */
