//  Ball.cpp
//  Aether_Zhang

#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include <iostream>

#include "MoveBalls.h"
#include "Ball.h"

bool Ball::init() {
    Sprite::initWithFile("ass/ball.png");   // Use a picture to represent the ball.
    data_.radius = (double)Sprite::getContentSize().width * 0.5f;
    return true;
}

// Initialize the default data of the ball.
Ball* Ball::initial_ball(BallData data) {
    Ball* p = create();
    assert(p != nullptr);
    p->setScale(data.radius / p->data_.radius);
    p->data_ = data;
    return p;
}

// random a int number from min to max.
inline int random(const int min, const int max) {
    return rand() % (max - min + 1) + min;
}

Ball* Ball::random_initial_ball(const cocos2d::Size& RP) {
    BallData data;
    srand((unsigned int)time(0));
    
    // Random the ball's data.
    data.radius = (double)random(MIN_RADIUS, MAX_RADIUS);
    data.weight = (double)random(MIN_WEIGHT, MAX_WEIGHT);
//    data.pos = cocos2d::Point((double)random(data.radius, RP.width - data.radius),
//                              (double)random(data.radius, RP.height - data.radius));
    data.pos = cocos2d::Point(0, 0);
    data.speed = cocos2d::Vec2((double)random(MIN_SPEED, MAX_SPEED),
                               (double)random(0, 2 * FLAT_ANGLE - 1) / FLAT_ANGLE * PI);
    data.world_stratum = 0u;
    return initial_ball(data);
}

const BallData& Ball::get() {
    return data_;
}

// Move the ball a distance.
void Ball::move(const double delta_x, const double delta_y) {
    data_.pos.x += delta_x;
    data_.pos.y += delta_y;
}

void Ball::modify_location(const cocos2d::Point& pos) {             // Change the ball's location.
    data_.pos = pos;
}

void Ball::modify_impulse(const cocos2d::Vec2& impulse) {           // Change the impulse received by the ball.
    // do something
}

void Ball::modify_speed(const cocos2d::Vec2& speed) {               // Change the speed of the current ball.
    data_.speed = speed;
}

void Ball::modify_world(const int world_id) {              // Change the ball's world.
    assert(world_id >= 0);
    data_.world_stratum = world_id;
}
