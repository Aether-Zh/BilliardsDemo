//
//  UIImagePickerAndroid.h
//  cocos2d_libs
//
//  Created by Joi Yao on 17/2/14.
//
//

#pragma once
#include "platform/CCPlatformConfig.h"

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "UIImagePickerImpl.h"

NS_CC_BEGIN

namespace ui {
    
    class ImagePicker;
    
    class ImagePickerImplAndroid : public ImagePickerImpl
    {
    public:
        ImagePickerImplAndroid(ImagePicker* pImagePicker);
        virtual ~ImagePickerImplAndroid();
        
        virtual void show(const int nRatioX,
                          const int nRatioY,
                          const int nWidth,
                          const int nHeight);
    };
}

NS_CC_END

//#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) */
