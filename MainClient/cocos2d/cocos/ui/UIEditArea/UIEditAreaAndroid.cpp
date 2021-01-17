//
//  UIEditAreaAndroid.cpp
//  cocos2d_libs
//
//  Created by Joi Yao on 16/12/29.
//
//

#include "UIEditAreaAndroid.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "UIEditArea.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#include "base/ccUTF8.h"

using namespace std;

NS_CC_BEGIN

namespace ui {
    
    EditAreaImpl* __createSystemEditArea(EditArea* pEditArea)
    {
        return new EditAreaImplAndroid(pEditArea);
    }
    
    EditAreaImplAndroid::EditAreaImplAndroid(EditArea* pEditText)
    : EditAreaImpl(pEditText)
    {
        
    }
    
    static void editAreaCallbackFunc(const char* pText, void* pEA)
    {
        EditAreaImplAndroid* thiz = (EditAreaImplAndroid*)pEA;
        log("EditAreaImplAndroid::editAreaCallbackFunc >>> %s", pText);
        if (thiz->getDelegate() != NULL)
        {
            thiz->getDelegate()->editAreaEditFinish(pText);
        }
    }
    
    EditAreaImplAndroid::~EditAreaImplAndroid()
    {
        log("EditAreaImplAndroid::~EditAreaImplAndroid");
        resetEditAreaDialog(editAreaCallbackFunc, (void*)this);
    }
    
    void EditAreaImplAndroid::showDialog(const string&  sTitle,
                                         const string&  sPlaceholder,
                                         const string&  sText,
                                         const int      nMaxLen,
                                         const float    fTitleHeight,
                                         const int      nTitleFont)
    {
        log("EditAreaImplAndroid::showDialog");
        showEditAreaDialogJNI(sTitle.c_str(),
                              sPlaceholder.c_str(),
                              sText.c_str(),
                              nMaxLen,
                              editAreaCallbackFunc,
                              (void*)this);
    }
    
}

NS_CC_END

#endif
