//
//  UIImagePickerImpl.h
//  cocos2d_libs
//
//  Created by Joi Yao on 17/2/8.
//
//

#pragma once

#include "UIImagePicker.h"

NS_CC_BEGIN

namespace ui{
    class ImagePickerImpl
    {
    public:
        ImagePickerImpl(ImagePicker* pImagePicker) : m_pDelegate(nullptr), m_pImagePicker(pImagePicker) {}
        virtual void show(const int nRatioX,
                          const int nRatioY,
                          const int nWidth,
                          const int nHeight) {};
        
        CC_SYNTHESIZE(ImagePickerDelegate*,    m_pDelegate,  Delegate);
        CC_SYNTHESIZE(ImagePicker*,            m_pImagePicker,    ImagePicker);
        virtual ~ImagePickerImpl() {}
    };
    
    // This method must be implemented at each subclass of ImagePickerImpl.
    extern ImagePickerImpl* __createSystemImagePicker(ImagePicker* pImagePicker);
}
NS_CC_END