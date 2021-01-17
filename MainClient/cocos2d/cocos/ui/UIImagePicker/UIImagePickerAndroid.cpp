//
//  UIImagePickerAndroid.cpp
//  cocos2d_libs
//
//  Created by Joi Yao on 17/2/14.
//
//

#include "UIImagePickerAndroid.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "UIImagePicker.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#include "base/ccUTF8.h"

using namespace std;

NS_CC_BEGIN

namespace ui {
    
    ImagePickerImpl* __createSystemImagePicker(ImagePicker* pImagePicker)
    {
        return new ImagePickerImplAndroid(pImagePicker);
    }
    
    ImagePickerImplAndroid::ImagePickerImplAndroid(ImagePicker* pEditText)
    : ImagePickerImpl(pEditText)
    {
        
    }
    
    static void imagePickerCallbackFunc(const char* pPath, bool bSucceed, void* pEA)
    {
        ImagePickerImplAndroid* thiz = (ImagePickerImplAndroid*)pEA;
        log("ImagePickerImplAndroid::ImagePickerCallbackFunc >>> %s, succeed=%s", pPath, bSucceed? "true": "false");
        if (thiz->getDelegate() != NULL)
        {
            thiz->getDelegate()->picked(pPath, bSucceed);
        }
    }
    
    ImagePickerImplAndroid::~ImagePickerImplAndroid()
    {
        log("ImagePickerImplAndroid::~ImagePickerImplAndroid");
        resetImagePicker(imagePickerCallbackFunc, (void*)this);
    }
    
    void ImagePickerImplAndroid::show(const int nRatioX,
                                      const int nRatioY,
                                      const int nWidth,
                                      const int nHeight)
    {
        log("ImagePickerImplAndroid::show");
        showImagePickerJNI(nRatioX,
                           nRatioY,
                           nWidth,
                           nHeight,
                           imagePickerCallbackFunc,
                           (void*)this);
    }
    
}

NS_CC_END

#endif
