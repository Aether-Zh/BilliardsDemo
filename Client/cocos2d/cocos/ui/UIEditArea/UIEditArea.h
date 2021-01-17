//
//  UIEditArea.h
//  cocos2d_libs
//
//  Created by Joi Yao on 16/12/29.
//
//

#pragma once

#include "cocos2d.h"

NS_CC_BEGIN

namespace ui
{
    class EditArea;
    class EditAreaImpl;
    
    class EditAreaDelegate
    {
    public:
        virtual ~EditAreaDelegate() {};
        virtual void editAreaEditFinish(const std::string& text) = 0;
    };

    
    class EditArea : public cocos2d::Ref
    {
    public:
        static EditArea* create(const std::string&  sTitle,
                                const std::string&  sPlaceholder,
                                const std::string&  sText,
                                const int           nMaxLen,
                                EditAreaDelegate*   pDelegate,
                                const float         fTitleHeight,
                                const int           nTitleFont);
        virtual bool init(const std::string&  sTitle,
                          const std::string&  sPlaceholder,
                          const std::string&  sText,
                          const int           nMaxLen,
                          EditAreaDelegate*   pDelegate,
                          const float         fTitleHeight,
                          const int           nTitleFont);
        EditArea();
        ~EditArea();
        CC_SYNTHESIZE_READONLY(EditAreaDelegate*, m_pDelegate, Delegate);
        
    private:
        void setDelegate(EditAreaDelegate* pDelegate);
        
    private:
        EditAreaImpl*   m_pEditAreaImpl;
    };
}

NS_CC_END