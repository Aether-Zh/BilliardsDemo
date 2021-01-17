//
//  UIEditAreaIOS.h
//  cocos2d_libs
//
//  Created by Joi Yao on 17/1/3.
//
//

#pragma once

#include "platform/CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "UIEditAreaImpl.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface EditAreaView : UIView<UITextViewDelegate>
{
    UIView *viewMenuBar_;
    UITextView* tvEdit_;
    UILabel* lblTitle_;
    UIButton* btnBack_;
    UIButton* btnDone_;
    int maxLength_;
    void* editArea_;
    float fScaleX_;
    float fScaleY_;
    float fScaleFit_;
}

@property(nonatomic, retain) UIView* viewMenuBar;
@property(nonatomic, retain) UITextView* tvEdit;
@property(nonatomic, retain) UILabel* lblTitle;
@property(nonatomic, retain) UIButton* btnBack;
@property(nonatomic, retain) UIButton* btnDone;
@property(nonatomic, assign) void* editArea;
@property(nonatomic, assign) int maxLength;
@property(nonatomic, assign) float fScaleX;
@property(nonatomic, assign) float fScaleY;
@property(nonatomic, assign) float fScaleFit;

@end

NS_CC_BEGIN

namespace ui {
    
    class EditArea;
    
    class EditAreaImplIOS : public EditAreaImpl
    {
    public:
        EditAreaImplIOS(EditArea* pEditArea);
        virtual ~EditAreaImplIOS();
        virtual void showDialog(const std::string& sTitle,
                                const std::string& sPlaceholder,
                                const std::string& sText,
                                const int          nMaxLen,
                                const float        fTitleHeight,
                                const int          nTitleFont);
        void clearView();
        
    private:
        EditAreaView* m_pEditAreaView;
    };
}

NS_CC_END
#endif