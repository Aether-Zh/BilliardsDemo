//  Ball.h
//  Aether_Zhang

#ifndef BALL_H
#define BALL_H

#include "cocos2d.h"

const double PI = 3.1415926f;           // PI ratio.                    (radian)


typedef struct {
    double radius;                 // ball's radius    (px)
    double weight;                 // ball's weight    (g)
    cocos2d::Point pos;            // ball's postion   (px, px)
    cocos2d::Vec2 speed;           // ball's speed     (px / ms, rad)
    unsigned int world_stratum;    // ball's world id  (int), world stratum can specify the world in which the ball is located.
} BallData;

static class Ball : public cocos2d::Sprite
{
public:
    virtual bool init();
    ~Ball() = default;
    
    Ball* initial_ball(BallData data);                        // Set the ball's data.
    Ball* random_initial_ball(const cocos2d::Size& RP);
    
    const BallData& get();                                         // Get the ball's data.
    
    void move(const double delta_x, const double delta_y);    // Move the ball a distance.
    
    void modify_location(const cocos2d::Point& pos);             // Change the ball's location.
    void modify_impulse(const cocos2d::Vec2& impulse);           // Change the impulse received by the ball.
    void modify_speed(const cocos2d::Vec2& speed);               // Change the speed of the current ball.
    void modify_world(const int world_id);                       // Change the ball's world.
    
    bool destroy_ = false;                                       // Wether exist.
private:
    CREATE_FUNC(Ball);
    
    const int FLAT_ANGLE = 180;             // Flat angle.                  (angle)
    
    const int MAX_SPEED = 10;               // Maximum speed.   (px / ms)
    const int MIN_SPEED = 5;                // Minimum speed.   (px / ms)
    
    const int MAX_RADIUS = 100;             // Maximum scale.   (px)
    const int MIN_RADIUS = 40;              // Minimum scale.   (px)
    
    const int MAX_WEIGHT = 10;              // Maximum weight.  (g)
    const int MIN_WEIGHT = 1;               // Minimum weight.  (g)
    
    BallData data_;                                              // Ball's information data.
} ball_factor;

#endif /* BALL_H */
