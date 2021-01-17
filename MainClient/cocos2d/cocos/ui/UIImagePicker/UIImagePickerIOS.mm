//
//  UIImagePickerIOS.m
//  cocos2d_libs
//
//  Created by Joi Yao on 17/2/8.
//
//


#include "UIImagePickerIOS.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "UIImagePicker.h"
#include "platform/CCFileUtils.h"
#import "platform/ios/CCEAGLView-ios.h"
#import <AVFoundation/AVFoundation.h>
#import <AssetsLibrary/AssetsLibrary.h>

#define getImagePickerImplIOS() ((cocos2d::ui::ImagePickerImplIOS*)imagePicker_)

@implementation TouchView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
    {
        self.backgroundColor = [UIColor grayColor];
    }
    return self;
}

- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    if (!self.isUserInteractionEnabled || self.isHidden || self.alpha <= 0.01)
    {
        return nil;
    }
    if ([self pointInside:point withEvent:event])
    {
        for (UIView *subview in [self.subviews reverseObjectEnumerator])
        {
            CGPoint convertedPoint = [subview convertPoint:point fromView:self];
            UIView *hitTestView = [subview hitTest:convertedPoint withEvent:event];
            if (hitTestView)
            {
                return hitTestView;
            }
        }
        [self touched];
        return self;
    }
    return nil;
}

- (void)touched
{
    if ([self.delegate respondsToSelector:@selector(touched:)])
    {
        [self.delegate touched:self];
    }
}
@end

@implementation ImagePickerController

- (BOOL) prefersStatusBarHidden
{
    return YES;
}

- (UIViewController *) childViewControllerForStatusBarHidden
{
    return nil;
}

@end

@implementation ImagePickerView

@synthesize imagePicker = imagePicker_;
@synthesize actionSheetExists = actionSheetExists_;
@synthesize ratioX = ratioX_;
@synthesize ratioY = ratioY_;
@synthesize width = width_;
@synthesize height = height_;

- (void)setImagePicker:(void *)value
{
    imagePicker_ = value;
}

- (void)setRatioX:(int)value
{
    ratioX_ = value;
}

- (void)setRatioY:(int)value
{
    ratioY_ = value;
}
- (void)setWidth:(int)value
{
    width_ = value;
}

-(void)setHeight:(int)value
{
    height_ = value;
}

-(void)setActionSheetExists:(BOOL)value
{
    actionSheetExists_ = value;
}

-(void)setCleared:(BOOL)value
{
    cleared_ = value;
}

-(UIInterfaceOrientationMask)supportedInterfaceOrientations
{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        return UIInterfaceOrientationMaskLandscape;
    }
    else
    {
        return UIInterfaceOrientationMaskAll;
    }
}

-(UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
{
    return [[UIApplication sharedApplication] statusBarOrientation];
}

-(void)viewDidLoad
{
    [super viewDidLoad];
    [self.view setBackgroundColor: [UIColor grayColor]];
}

-(void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    [self createActionSheet];
    if (cleared_)
    {
        [self clearView];
    }
}

-(void)createActionSheet
{
    if (actionSheetExists_)
    {
        return;
    }
    UIActionSheet *actionSheet = [[UIActionSheet alloc] initWithTitle:@""
                                                             delegate:self
                                                    cancelButtonTitle:@"暂时不"
                                               destructiveButtonTitle:nil
                                                    otherButtonTitles:@"照相", @"选取现有图片", nil];
    actionSheet.actionSheetStyle = UIActionSheetStyleBlackTranslucent;
    actionSheet.delegate = self;
    [actionSheet showInView: self.view];
    [actionSheet release];
    actionSheetExists_ = YES;
}

-(void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
    [actionSheet dismissWithClickedButtonIndex:0 animated:YES];
    ImagePickerController *imgPickerController = [[ImagePickerController alloc] init];
    imgPickerController.view.frame = self.view.frame;
    imgPickerController.delegate = self;
    imgPickerController.allowsEditing = YES;
    BOOL showImgPicker = NO;
    switch (buttonIndex)
    {
        case 0:
        {
            AVAuthorizationStatus authStatus = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
            if (authStatus != AVAuthorizationStatusAuthorized && authStatus != AVAuthorizationStatusNotDetermined)
            {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"无法使用相机"
                                                                message:@"请在\"设置-隐私-相机\"中允许访问相机。"
                                                               delegate:self
                                                      cancelButtonTitle:@"确定"
                                                      otherButtonTitles:nil];
                [alert show];
                [alert release];
                return;
            }
            if ([UIImagePickerController isSourceTypeAvailable: UIImagePickerControllerSourceTypeCamera])
            {
                imgPickerController.sourceType = UIImagePickerControllerSourceTypeCamera;
                imgPickerController.showsCameraControls = YES;
                showImgPicker = YES;
            }
            else
            {
                NSLog(@"模拟器无法打开相机");
                [self picked:nil :NO];
            }
        }
            break;
        case 1:
        {
            ALAuthorizationStatus authStatus = [ALAssetsLibrary authorizationStatus];
            if(authStatus != ALAuthorizationStatusAuthorized && authStatus != ALAuthorizationStatusNotDetermined)
            {
                
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"无法使用相册"
                                                                message:@"请在\"设置-隐私-照片\"中允许访问照片。"
                                                               delegate:self
                                                      cancelButtonTitle:@"确定"
                                                      otherButtonTitles:nil];
                [alert show];
                [alert release];
                return;
            }
            imgPickerController.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
            showImgPicker = YES;
        }
            break;
        default:
            [self picked:nil :NO];
            break;
    }
    if (showImgPicker)
    {
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
        {
            [[NSOperationQueue mainQueue] addOperationWithBlock:^{
                TouchView *view = [[TouchView alloc]initWithFrame:self.view.frame];
                [self.view addSubview:view];
                view.delegate = self;
                UIPopoverController *popover = [[UIPopoverController alloc] initWithContentViewController:imgPickerController];
                popover.delegate = self;
                popover.passthroughViews = @[view];
                CGSize sizeAll = self.view.bounds.size;
                imgPickerController.preferredContentSize = sizeAll;
                popover.popoverContentSize = sizeAll;
                [popover presentPopoverFromRect:CGRectZero inView:self.view permittedArrowDirections:0 animated:YES];
                [popover release];
            }];
        }
        else
        {
            [self presentViewController:imgPickerController animated:YES completion:nil];
        }
    }
    [imgPickerController release];
}

- (void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex
{
    [self picked:nil :NO];
}

-(void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    [picker dismissViewControllerAnimated:YES completion:nil];
    UIImage* image = [info objectForKey:UIImagePickerControllerEditedImage];
    [self saveImage:image];
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [picker dismissViewControllerAnimated:YES completion:nil];
    [self picked:nil :NO];
}

- (BOOL)popoverControllerShouldDismissPopover:(UIPopoverController *)popoverController
{
    return NO;
}

- (void)touched:(TouchView *)view
{
    [view removeFromSuperview];
    [self picked:nil :NO];
}

- (void)saveImage:(UIImage *)image
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *filepath = [paths objectAtIndex:0];
    BOOL succeed = NO;
    if (filepath.length > 0)
    {
        filepath = [filepath stringByAppendingString:@"/tmp.jpg"];
        CGSize size = CGSizeMake(width_, height_);
        image  = [self scaleImage:image scaledToSize:size];
        NSData *imageData = UIImageJPEGRepresentation(image, 0.75);
        [[NSFileManager defaultManager]removeItemAtPath:filepath error:nil];
        succeed = [imageData writeToFile:filepath atomically:YES];
    }
    [self picked:filepath :succeed];
}

//压缩图片
- (UIImage*)scaleImage:(UIImage*)image scaledToSize:(CGSize)newSize
{
    UIGraphicsBeginImageContext(newSize);
    [image drawInRect:CGRectMake(0,0,newSize.width,newSize.height)];
    UIImage* newImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return newImage;
}

-(void) picked:(NSString*)filepath :(BOOL)succeed
{
    std::string sPath = filepath? filepath.UTF8String: "";
    getImagePickerImplIOS()->getDelegate()->picked(sPath, succeed);
    [self clearView];
}

-(void) clearView
{
    for(UIView *view in [self.view subviews])
    {
        [view removeFromSuperview];
    }
    [self dismissViewControllerAnimated:YES completion:nil];
    cleared_ = YES;
}
@end

NS_CC_BEGIN
namespace ui {
    
    ImagePickerImpl* __createSystemImagePicker(ImagePicker* pImagePicker)
    {
        return new ImagePickerImplIOS(pImagePicker);
    }
    
    ImagePickerImplIOS::ImagePickerImplIOS(ImagePicker* pImagePicker)
    : cocos2d::ui::ImagePickerImpl(pImagePicker)
    {
        m_pImagePickerView = nullptr;
    }
    
    ImagePickerImplIOS::~ImagePickerImplIOS()
    {
        if (m_pImagePickerView)
        {
            [m_pImagePickerView setImagePicker:nullptr];
        }
        clearView();
        m_pImagePickerView = nullptr;
        m_pDelegate = nullptr;
        m_pImagePicker = nullptr;
    }
    
    void ImagePickerImplIOS::show(const int nRatioX,
                                  const int nRatioY,
                                  const int nWidth,
                                  const int nHeight)
    {
        auto glview = cocos2d::Director::getInstance()->getOpenGLView();
        Size size = glview->getFrameSize();
        CGRect rect = CGRectMake(0, 0, size.width, size.height);
        
        CCEAGLView *eaglview = static_cast<CCEAGLView *>(glview->getEAGLView());
        float factor = eaglview.contentScaleFactor;
        rect.size.width /= factor;
        rect.size.height /= factor;
        
        clearView();
        
        m_pImagePickerView = [[ImagePickerView alloc] init];
        [m_pImagePickerView setImagePicker:this];
        [m_pImagePickerView setRatioX:nRatioX];
        [m_pImagePickerView setRatioY:nRatioY];
        [m_pImagePickerView setWidth:nWidth];
        [m_pImagePickerView setHeight:nHeight];
        [m_pImagePickerView setActionSheetExists:NO];
        [m_pImagePickerView setCleared:NO];
        [m_pImagePickerView retain];
        UIViewController *rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
        [rootViewController presentViewController:m_pImagePickerView animated:YES completion:nil];
    }
    
    void ImagePickerImplIOS::clearView()
    {
        if (m_pImagePickerView)
        {
            [m_pImagePickerView setImagePicker:nullptr];
            [m_pImagePickerView clearView];
            [m_pImagePickerView release];
            m_pImagePickerView = nullptr;
        }
    }
}
NS_CC_END
#endif
