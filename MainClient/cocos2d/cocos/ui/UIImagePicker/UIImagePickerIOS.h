//
//  UIImagePickerIOS.h
//  cocos2d_libs
//
//  Created by Joi Yao on 17/2/8.
//
//

#include "platform/CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "UIImagePickerImpl.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@protocol TouchViewDelegate;

@interface TouchView : UIView
@property (nonatomic, assign) id<TouchViewDelegate> delegate;
@end

@protocol TouchViewDelegate <NSObject>
@optional
- (void)touched:(TouchView *)view;
@end

@interface ImagePickerController : UIImagePickerController
- (BOOL) prefersStatusBarHidden;
@end

@protocol ImagePickerDelegate;
@interface ImagePickerView : UIViewController<UIActionSheetDelegate, UIImagePickerControllerDelegate, UINavigationControllerDelegate, UIPopoverControllerDelegate, TouchViewDelegate>
{
    void* imagePicker_;
    BOOL actionSheetExists_;
    BOOL cleared_;
    int ratioX_;
    int ratioY_;
    int width_;
    int height_;
}

@property(nonatomic, assign) void*                      imagePicker;
@property(nonatomic, assign) BOOL                       actionSheetExists;
@property(nonatomic, assign) BOOL                       cleared;
@property(nonatomic, assign) int                        ratioX;
@property(nonatomic, assign) int                        ratioY;
@property(nonatomic, assign) int                        width;
@property(nonatomic, assign) int                        height;


@end
NS_CC_BEGIN

namespace ui {
    
    class ImagePicker;
    
    class ImagePickerImplIOS : public ImagePickerImpl
    {
    public:
        ImagePickerImplIOS(ImagePicker* pImagePicker);
        virtual ~ImagePickerImplIOS();
        virtual void show(const int nRatioX,
                          const int nRatioY,
                          const int nWidth,
                          const int nHeight);
        void clearView();
        
    private:
        ImagePickerView* m_pImagePickerView;
    };
}

NS_CC_END
#endif
