#include <stdlib.h>
#include <assert.h>

#include <thread>
#include <chrono>
#include <string>

#include "MoveBalls.h"
#include "cocos2d.h"
#include "Ball.h"
#include "Physical.h"
#include "ui/CocosGUI.h"
#include "Transmission.h"
#include "MessageHeader.h"

std::vector<std::string> split_char(const std::string& s, char ch) {
    std::vector<std::string> splits;
    std::string t;
    for (auto& i : s) {
        if (i != ch) {
            t += i;
        }else if (!t.empty()){
            splits.emplace_back(t);
            t.clear();
        }
    }
    if (!t.empty())
        splits.emplace_back(t);
    return splits;
}

cocos2d::Scene* MoveBalls::createScene() {
    auto scene = cocos2d::Scene::create();
    auto layer = MoveBalls::create();
    scene->addChild(layer);
    return scene;
}

bool MoveBalls::init() {
    cocos2d::Sprite* pBall = cocos2d::Sprite::create("ass/ball.png");

    pBall->setPosition(300, 500);
    pBall->setVisible(true);
    addChild(pBall);
    
    auto scaleTo1 = cocos2d::ScaleTo::create(0.25f, 1.2f);
    auto scaleTo2 = cocos2d::ScaleTo::create(0.25f, 1.0f);
    auto scaleTo3 = cocos2d::ScaleTo::create(0.25f, 1.0f);
    
    auto call_back = cocos2d::CallFunc::create([&](){
        printf("w: %f, h: %f\n", pBall->getContentSize().width, pBall->getContentSize().height);
    });
    
    auto seq = cocos2d::Sequence::create(scaleTo1, call_back, scaleTo2, call_back, scaleTo3, call_back, nullptr);
    
    pBall->runAction(cocos2d::RepeatForever::create(seq));
//    if (!Layer::init()) {
//        return false;
//    }
//
//    // Connect the socket.
//    if (!transporter.init() || !transporter.cnct(IP, PORT)) {
//        puts("no cnct");
//    }
    
//    double w = getContentSize().width, h = getContentSize().height;
//    double w_scale = w / WIDTH, h_scale = h / HEIGHT;
//
//    scale_ = abs(w_scale - 1) > abs(h_scale - 1) ? w_scale : h_scale;
//    offset_ = std::make_pair((w - scale_ * WIDTH) / 2.0f,
//                             (h - scale_ * HEIGHT) / 2.0f);
//
//    auto listener = cocos2d::EventListenerTouchOneByOne::create();  // Create a event listener.
//    listener->onTouchBegan = CC_CALLBACK_2(MoveBalls::onTouchBegan, this);  // Define the callback function.
//    listener->onTouchMoved = CC_CALLBACK_2(MoveBalls::onTouchMoved, this);  // Define the callback function.
//    listener->onTouchEnded = CC_CALLBACK_2(MoveBalls::onTouchEnded, this);  // Define the callback function.
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);   // Register the function.
//
//    std::thread(std::mem_fn(&MoveBalls::recv_task), this).detach();

    return true;
}

// Send the postion.
bool MoveBalls::onTouchBegan(cocos2d::Touch* tTouch, cocos2d::Event* eEvent) {
    cocos2d::Point pos = tTouch->getLocation();
    bool is_choise = false;
    Ball* pb = nullptr;
    for (auto& i : getChildren()) {
        pb = (Ball*)i;
        if (physical::distance(pb->getPositionX(), pb->getPositionY(), pos.x, pos.y) < pb->r_) {
            is_choise = true;
            cur_ball_p_ = pb;
            printf("%s\n", cur_ball_p_->getName().c_str());
            break;
        }
    }
    if (!is_choise && cur_ball_p_ == nullptr) {
        Msg msg;
        CreateBall create_ball_info;
        
        create_ball_info.set_id(id_++);
        create_ball_info.set_x(pos.x / scale_ - offset_.first);
        create_ball_info.set_y(pos.y / scale_ - offset_.second);
        
        msg.first.type = htons(CREATE_CMD_TYPE);
        msg.second = create_ball_info.SerializeAsString();
        msg.first.length = htons(msg.second.size());
        
        transporter.sendmsg(msg);
    }else if (!is_choise && cur_ball_p_ != nullptr) {
        printf("x: %f y: %f\n", pos.x, pos.y);
    }
    
    return true;
}

void MoveBalls::onTouchMoved(cocos2d::Touch* tTouch, cocos2d::Event* eEvent) {
    cocos2d::Point pos = tTouch->getLocation();
    if (cur_ball_p_ != nullptr) {
        printf("x: %f y: %f\n", pos.x, pos.y);
    }
}

void MoveBalls::onTouchEnded(cocos2d::Touch* tTouch, cocos2d::Event* eEvent) {
    cocos2d::Point pos = tTouch->getLocation();
    if (cur_ball_p_ != nullptr) {
        printf("x: %f y: %f\n", pos.x, pos.y);
        puts("boob!");
        std::vector<std::string>&& sv = split_char(cur_ball_p_->getName(), '-');
        Msg msg;
        MoveBall move_ball;
        move_ball.set_sock(std::stoi(sv[0]));
        move_ball.set_id(std::stoi(sv[1]));
        move_ball.set_v_val(10.0f);
        move_ball.set_v_dir(atan2(cur_ball_p_->getPositionY() - pos.y, cur_ball_p_->getPositionX() - pos.x));
        
        msg.first.type = htons(MOVE_CMD_TYPE);
        msg.second = move_ball.SerializeAsString();
        msg.first.length = htons(msg.second.size());
        
        transporter.sendmsg(msg);
        cur_ball_p_ = nullptr;
    }
}

void MoveBalls::recv_task() {
    Msgs msgs;
    std::string* msg_ptr;
    
    for (;;) {
        transporter.recvmsg(msgs);
        for (auto& i : msgs) {
            msg_ptr = new std::string(i.second);
            if (i.first.type == DATA_TYPE) {
                work_queue_.PushItem(this, workQ_selector(MoveBalls::show_ball), (void*)msg_ptr);
            }else if (i.first.type == RM_CMD_TYPE) {
                work_queue_.PushItem(this, workQ_selector(MoveBalls::rm_ball), (void*)msg_ptr);
            }
        }
        msgs.clear();
        std::this_thread::sleep_for(std::chrono::microseconds(1000000 / TPS));
    }
}

void MoveBalls::rm_ball(void* msg_ptr) {
    std::string* msg_p = static_cast<std::string*>(msg_ptr);
    Ball* ball_ptr;
    RemoveBall rm_ball_info;
    
    rm_ball_info.ParseFromString(*msg_p);
    
    while ((ball_ptr = (Ball*)getChildByTag(rm_ball_info.sock())) != nullptr) {
        ball_ptr->removeFromParent();
    }
}

void MoveBalls::show_ball(void* msg_ptr) {
    std::string* msg_p = static_cast<std::string*>(msg_ptr);
    Ball* ball_ptr;
    BallsInfo balls_info;
    int sock, id;
    
    balls_info.ParseFromString(*msg_p);
    
    for (int i = 0; i < balls_info.ball_size(); ++i) {
        sock = balls_info.ball(i).sock();
        id = balls_info.ball(i).id();
        
        if ((ball_ptr = (Ball*)getChildByName(std::to_string(sock) + "-" + std::to_string(id))) == nullptr) {
            ball_ptr = Ball::create();
            assert(ball_ptr != nullptr);
            
            ball_ptr->r_ = balls_info.ball(i).r() * scale_;
            
            ball_ptr->setName(std::to_string(sock) + "-" + std::to_string(id));
            ball_ptr->setTag(sock);
            ball_ptr->setScale(2.0f * ball_ptr->r_ / ball_ptr->getContentSize().width);
            ball_ptr->setVisible(true);
            
            this->addChild(ball_ptr);
        }
        ball_ptr->setPosition(balls_info.ball(i).x() * scale_ + offset_.first,
                              balls_info.ball(i).y() * scale_ + offset_.second);
    }
    
    delete(msg_p);
}
