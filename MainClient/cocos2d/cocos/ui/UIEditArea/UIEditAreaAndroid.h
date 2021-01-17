//
//  UIEditAreaAndroid.h
//  cocos2d_libs
//
//  Created by Joi Yao on 16/12/29.
//
//

#pragma once

#include "platform/CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "UIEditAreaImpl.h"

NS_CC_BEGIN

namespace ui {
    
    class EditArea;
    
    class EditAreaImplAndroid : public EditAreaImpl
    {
    public:
        EditAreaImplAndroid(EditArea* pEditText);
        virtual ~EditAreaImplAndroid();
        
        virtual void showDialog(const std::string& sTitle,
                                const std::string& sPlaceholder,
                                const std::string& sText,
                                const int          nMaxLen,
                                const float        fTitleHeight,
                                const int          nTitleFont);
    };
}

NS_CC_END

#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) */
