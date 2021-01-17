//
//  UIImagePicker.cpp
//  cocos2d_libs
//
//  Created by Joi Yao on 17/2/8.
//
//

#include "UIImagePicker.h"
#include "UIImagePickerImpl.h"

using namespace std;
USING_NS_CC;

NS_CC_BEGIN

namespace ui
{
    ImagePicker* ImagePicker::create(const int nRatioX,
                                     const int nRatioY,
                                     const int nWidth,
                                     const int nHeight,
                                     cocos2d::ui::ImagePickerDelegate *pDelegate)
    {
        ImagePicker* pRet = new ImagePicker();
        if (pRet && pRet->init(nRatioX, nRatioY, nWidth, nHeight, pDelegate))
        {
            pRet->autorelease();
            pRet->setDelegate(pDelegate);
        }
        else
        {
            CC_SAFE_DELETE(pRet);
            pRet = nullptr;
        }
        return pRet;
    }
    
    bool ImagePicker::init(const int nRatioX,
                           const int nRatioY,
                           const int nWidth,
                           const int nHeight,
                           ui::ImagePickerDelegate *pDelegate)
    {
        m_pDelegate = pDelegate;
        m_pImagePickerImpl = __createSystemImagePicker(this);
        setDelegate(pDelegate);
        m_pImagePickerImpl->show(nRatioX, nRatioY, nWidth, nHeight);
        return true;
    }
    
    ImagePicker::ImagePicker()
    :
    m_pImagePickerImpl(nullptr),
    m_pDelegate(nullptr)
    {
    }
    
    ImagePicker::~ImagePicker()
    {
        m_pDelegate = nullptr;
        if (m_pImagePickerImpl)
        {
            m_pImagePickerImpl->setDelegate(nullptr);
            CC_SAFE_DELETE(m_pImagePickerImpl);
            m_pImagePickerImpl = nullptr;
        }
    }
    
    void ImagePicker::setDelegate(ui::ImagePickerDelegate *pDelegate)
    {
        m_pDelegate = pDelegate;
        if (m_pDelegate && m_pImagePickerImpl)
        {
            m_pImagePickerImpl->setDelegate(pDelegate);
        }
    }
}

NS_CC_END
