#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include <vector>
#include <iostream>
#include <thread>
#include <chrono>

#include "MoveBalls.h"
#include "cocos2d.h"
#include "Ball.h"
#include "Physical.h"
#include "ui/CocosGUI.h"
#include "balls_info.pb.h"
#include "Transmission.h"
#include "MessageHeader.h"

cocos2d::Scene* MoveBalls::createScene() {
    // 'scene' is an autorelease object
    auto scene = cocos2d::Scene::create();
    // 'layer' is an autorelease object
    auto layer = MoveBalls::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

bool MoveBalls::init() {
    if (!Layer::init()) {
        return false;
    }
    
    scene_size_ = this->getContentSize();   // Get the scene's width and height.
    
    init_touch_event();                           // Initlization the touch action.
    init_button();                          // Initlization the clear button.
    
    std::thread painter(std::mem_fn(&MoveBalls::paint_task), this); // Refresh all ball's postion.
    std::thread sender(std::mem_fn(&MoveBalls::send_task), this);   // Send all ball's postion.
    painter.detach();
    sender.detach();
    
    return true;
}

// Check whether the two balls crashing.
inline int MoveBalls::crash(Ball* pb1, Ball* pb2) {
    if (pb1 && pb2) {
        double d = physical::distance(pb1->get().pos, pb2->get().pos);
        double r1 = pb1->get().radius, r2 = pb2->get().radius;
        if (d > ACCURACY && d < r1 + r2) return SUC;
    }
    return FAIL;
}

// Check whether the ball in scence.
inline int MoveBalls::crash(Ball* pb, const std::vector<cocos2d::Point>& vertex, const cocos2d::Point& center) {
    //    for (int i = 0; i < vertex.size(); ++i) {
    //
    //    }
    // Return 0, 1, 2, 3, 4, 0 is not crash, 1 is left, 2 is top, 3 is right, 4 is bottom.
    //       2
    //    \____/
    //    |\  /|
    //  1 | \/ |3
    //    | /\ |
    //    |/__\|
    //    /  4 \
    
    return 0;
}

void MoveBalls::init_touch_event() {
    // Create a event listener.
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    // Define the callback function.
    listener->onTouchBegan = CC_CALLBACK_2(MoveBalls::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MoveBalls::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MoveBalls::onTouchEnded, this);
    // Register the function.
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void MoveBalls::init_button() {
    pbutton_ = cocos2d::ui::Button::create("ass/ball.png", "ass/ball.png", "ass/ball.png");
    pbutton_->setPosition(cocos2d::Point(0.0f, 0.0f));
    pbutton_->setScale(0.5f);
    
    pbutton_->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                for (auto& i : balls_) {
                    mtx_.lock();
                    i->destroy_ = true;
                    i = nullptr;
                    mtx_.unlock();
                }
                
                mtx_.lock();
                balls_.clear();
                mtx_.unlock();
                break;
            default:
                break;
        }
    });
    
    this->addChild(pbutton_);
}

bool MoveBalls::onTouchBegan(cocos2d::Touch* tTouch, cocos2d::Event* eEvent) {
    if (pball_ == nullptr && (pball_ = ball_factor.random_initial_ball(scene_size_)) != nullptr) {
        pball_->setPosition(pball_->get().pos);
        // Add ball to parent.
        addChild(pball_);
        return true;
    }
    return false;
}

void MoveBalls::onTouchMoved(cocos2d::Touch* tTouch, cocos2d::Event* eEvent) {
    if (pball_ != nullptr) {
        pball_->setPosition(tTouch->getLocation());
    }
}

void MoveBalls::onTouchEnded(cocos2d::Touch* tTouch, cocos2d::Event* eEvent) {
    if (pball_ != nullptr) {
        pball_->modify_location(tTouch->getLocation());
    } else {
        return;
    }

    bool is_crash = false;
    for (int i = 0; i < balls_.size(); ++i) {
        if (crash(pball_, balls_[i])) {
            is_crash = true;
            break;
        }
    }
    
    // If the ball is outside the boundary or crash with the others, remove it from the parent.
    if (pball_->get().pos.x < pball_->get().radius ||
        pball_->get().pos.y < pball_->get().radius ||
        pball_->get().pos.x > scene_size_.width - pball_->get().radius ||
        pball_->get().pos.y > scene_size_.height - pball_->get().radius ||
        is_crash) {
        pball_->removeFromParent();     // Remove this ball.
    }else {
        mtx_.lock();
        balls_.emplace_back(pball_);    // Push this ball.
        mtx_.unlock();
        
        // Use thread to calculate the ball's postion all the time.
        std::thread(std::mem_fn(&MoveBalls::calc_task), this, pball_).detach();
    }
    pball_ = nullptr;
}

void MoveBalls::calc_task(Ball* ball) {
    double l = ball->get().radius;                          // left
    double r = scene_size_.width - ball->get().radius;      // right
    double t = scene_size_.height - ball->get().radius;     // top
    double b = ball->get().radius;                          // bottom
    
    BallData t_data;
    double t_angle, t_x, t_y;
    
    for (; ball->destroy_ == false;) {
        auto start = std::chrono::steady_clock::now();
        // Crash check.
        
        // Boundaries crash check.
        // left
        if (ball->get().pos.x < l) {
            ball->modify_speed(cocos2d::Vec2(ball->get().speed.x, -ball->get().speed.y + PI));
            while (ball->get().pos.x < l) {
                ball->move(1.0f, 0.0f);
            }
        }
        // right
        if (ball->get().pos.x > r) {
            ball->modify_speed(cocos2d::Vec2(ball->get().speed.x, -ball->get().speed.y + PI ));
            while (ball->get().pos.x > r) {
                ball->move(-1.0f, 0.0f);
            }
        }
        // top
        if (ball->get().pos.y > t) {
            ball->modify_speed(cocos2d::Vec2(ball->get().speed.x, -ball->get().speed.y));
            while (ball->get().pos.y > t) {
                ball->move(0.0f, -1.0f);
            }
        }
        // bottom
        if (ball->get().pos.y < b) {
            ball->modify_speed(cocos2d::Vec2(ball->get().speed.x, -ball->get().speed.y));
            while (ball->get().pos.y < b) {
                ball->move(0.0f, 1.0f);
            }
        }
        
        // Balls crash check.
        
        for (int i = 0; i < balls_.size(); ++i) {
            mtx_.lock();
            if (!balls_.empty() && crash(balls_[i], ball) == SUC) {
                // Swap the speed.
                t_data = balls_[i]->get();
                balls_[i]->modify_speed(ball->get().speed);
                mtx_.unlock();
                ball->modify_speed(t_data.speed);
                
                // Angle formed by two points.
                t_angle = atan2(ball->get().pos.y - t_data.pos.y,
                                ball->get().pos.x - t_data.pos.x);
                
                t_x = cos(t_angle);
                t_y = sin(t_angle);
                
                while (!balls_.empty() && crash(balls_[i], ball) == SUC) {
                    ball->move(t_x, t_y);
                    if (ball->get().pos.x < l || ball->get().pos.x > r || ball->get().pos.y > t || ball->get().pos.y < b) {
                        break;
                    }
                }
                break;
            }else {
                mtx_.unlock();
            }
        }
        
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::micro> elapsed = end - start;
        
        ball->move(ball->get().speed.x * cos(ball->get().speed.y),
                   ball->get().speed.x * sin(ball->get().speed.y));
        
        if (elapsed.count() < CALC_T) {
            std::this_thread::sleep_for(std::chrono::microseconds(CALC_T) - elapsed);
        }
    }
    
    mtx_.lock();
    ball->removeFromParent();
    mtx_.unlock();
    
    ball = nullptr;
}

void MoveBalls::paint_task() {
    for (;;) {
        for (int i = 0; i < balls_.size(); ++i) {
            mtx_.lock();
            if (!balls_.empty() && balls_[i] != nullptr) {
                balls_[i]->setPosition(balls_[i]->get().pos);                       // Display the ball in scene.
            }
            mtx_.unlock();
        }
        std::this_thread::sleep_for(std::chrono::microseconds(1000000 / FPS));      // FPS
    }
}

void MoveBalls::send_task() {
    if (transporter.init() && transporter.cnct(IP, PORT)) {
        printf("connect successed!\n");
    } else {
        printf("connect failed!\n");
        return;
    }

    BallData ball_data;
    BallInfo* pball_info;
    BallsInfo balls_info;
    for (;;) {
        for (int i = 0; i < balls_.size(); ++i) {
            mtx_.lock();
            if (balls_.empty() || balls_[i] == nullptr) {
                mtx_.unlock();
            } else {
                ball_data = balls_[i]->get();
                
                mtx_.unlock();
                
                pball_info = balls_info.add_ball();
                pball_info->set_id(i);
                pball_info->set_r(ball_data.radius);
                pball_info->set_x(ball_data.pos.x);
                pball_info->set_y(ball_data.pos.y);
            }
        }
        transporter.sendmsg(balls_info);
        balls_info.clear_ball();
        std::this_thread::sleep_for(std::chrono::microseconds(1000000 / TPS));      // FPS
    }
}
