//
//  UIImagePicker.h
//  cocos2d_libs
//
//  Created by Joi Yao on 17/2/8.
//
//

#pragma once
#include "cocos2d.h"

NS_CC_BEGIN

namespace ui
{
    class ImagePicker;
    class ImagePickerImpl;
    
    class ImagePickerDelegate
    {
    public:
        virtual ~ImagePickerDelegate() {};
        virtual void picked(const std::string& sFilePath, bool bSucceed) = 0;
    };
    
    
    class ImagePicker : public cocos2d::Ref
    {
    public:
        static ImagePicker* create(const int nRatioX,
                                   const int nRatioY,
                                   const int nWidth,
                                   const int nHeight,
                                   ImagePickerDelegate* pDelegate);
        virtual bool init(const int nRatioX,
                          const int nRatioY,
                          const int nWidth,
                          const int nHeight,
                          ImagePickerDelegate* pDelegate);
        ImagePicker();
        ~ImagePicker();
        CC_SYNTHESIZE_READONLY(ImagePickerDelegate*, m_pDelegate, Delegate);
        
    private:
        void setDelegate(ImagePickerDelegate* pDelegate);
        
    private:
        ImagePickerImpl*   m_pImagePickerImpl;
    };
}

NS_CC_END