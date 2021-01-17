//  Ball.cpp
//  Aether_Zhang

#include "cocos2d.h"
#include "Ball.h"

bool Ball::init() {
    Sprite::setVisible(false);
    Sprite::initWithFile("ass/ball.png");   // Use a picture to represent the ball.
    return true;
}
//
//void Ball::show() {
//    Sprite::setScale(2.0f * r_ / Sprite::getContentSize().width);
//    Sprite::setPosition(x_, y_);
//}
