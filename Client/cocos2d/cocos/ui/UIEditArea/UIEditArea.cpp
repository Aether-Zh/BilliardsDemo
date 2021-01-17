//
//  EditArea.cpp
//  cocos2d_libs
//
//  Created by Joi Yao on 16/12/29.
//
//

#include "UIEditArea.h"
#include "UIEditAreaImpl.h"

using namespace std;
USING_NS_CC;

NS_CC_BEGIN

namespace ui
{
    EditArea* EditArea::create(const string&            sTitle,
                               const string&            sPlaceholder,
                               const string&            sText,
                               const int                nMaxLen,
                               ui::EditAreaDelegate*    pDelegate,
                               const float              fTitleHeight,
                               const int                nTitleFont)
    {
        EditArea* pRet = new EditArea();
        if (pRet && pRet->init(sTitle, sPlaceholder, sText, nMaxLen, pDelegate, fTitleHeight, nTitleFont))
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
    
    bool EditArea::init(const string&           sTitle,
                        const string&           sPlaceholder,
                        const string&           sText,
                        const int               nMaxLen,
                        ui::EditAreaDelegate*   pDelegate,
                        const float             fTitleHeight,
                        const int               nTitleFont)
    {
        m_pDelegate = pDelegate;
        
        m_pEditAreaImpl = __createSystemEditArea(this);
        setDelegate(pDelegate);
        m_pEditAreaImpl->showDialog(sTitle, sPlaceholder, sText, nMaxLen, fTitleHeight, nTitleFont);
        return true;
    }
    
    EditArea::EditArea()
    :
    m_pEditAreaImpl(nullptr),
    m_pDelegate(nullptr)
    {
    }
    
    EditArea::~EditArea()
    {
        m_pDelegate = nullptr;
        if (m_pEditAreaImpl)
        {
            m_pEditAreaImpl->setDelegate(nullptr);
            CC_SAFE_DELETE(m_pEditAreaImpl);
            m_pEditAreaImpl = nullptr;
        }
    }
    
    void EditArea::setDelegate(ui::EditAreaDelegate *pDelegate)
    {
        m_pDelegate = pDelegate;
        if (m_pDelegate && m_pEditAreaImpl)
        {
            m_pEditAreaImpl->setDelegate(pDelegate);
        }
    }
}

NS_CC_END
