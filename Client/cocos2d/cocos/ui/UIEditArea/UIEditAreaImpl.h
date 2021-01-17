//
//  UIEditAreaImpl.h
//  cocos2d_libs
//
//  Created by Joi Yao on 17/1/3.
//
//

#pragma once

#include "UIEditArea.h"

NS_CC_BEGIN

namespace ui{
    class EditAreaImpl
    {
    public:
        EditAreaImpl(EditArea* pEditArea) : m_pDelegate(nullptr), m_pEditArea(pEditArea) {}
        virtual void showDialog(const std::string& sTitle,
                                const std::string& sPlaceholder,
                                const std::string& sText,
                                const int          nMaxLen,
                                const float        fTitleHeight,
                                const int          nTitleFont) {};
        
        CC_SYNTHESIZE(EditAreaDelegate*,    m_pDelegate, Delegate);
        CC_SYNTHESIZE(EditArea*,            m_pEditArea, EditArea);
        virtual ~EditAreaImpl() {}
    };
    
    // This method must be implemented at each subclass of EditAreaImpl.
    extern EditAreaImpl* __createSystemEditArea(EditArea* pEditArea);
}
NS_CC_END