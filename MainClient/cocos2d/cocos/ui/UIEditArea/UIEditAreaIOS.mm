//
//  UIEditAreaIOS.mm
//  cocos2d_libs
//
//  Created by Joi Yao on 17/1/3.
//
//

#include "UIEditAreaIOS.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "UIEditArea.h"
#import "platform/ios/CCEAGLView-ios.h"

#define getEditAreaImplIOS() ((cocos2d::ui::EditAreaImplIOS*)editArea_)
#define COLOR(R,G,B,A) [UIColor colorWithRed:R/255.0 green:G/255.0 blue:B/255.0 alpha:A]
#define TAG_BACK 100
#define TAG_DONE 200

@implementation EditAreaView

@synthesize viewMenuBar = viewMenuBar_;
@synthesize tvEdit = tvEdit_;
@synthesize lblTitle = lblTitle_;
@synthesize btnBack = btnBack_;
@synthesize btnDone = btnDone_;
@synthesize editArea = editArea_;
@synthesize maxLength = maxLength_;
@synthesize fScaleX = fScaleX_;
@synthesize fScaleY = fScaleY_;
@synthesize fScaleFit = fScaleFit_;

- (void)dealloc
{
    [viewMenuBar_ removeFromSuperview];
    self.viewMenuBar = NULL;
    [tvEdit_ resignFirstResponder];
    [tvEdit_ removeFromSuperview];
    self.tvEdit = NULL;
    [lblTitle_ removeFromSuperview];
    self.lblTitle = NULL;
    [btnBack_ removeFromSuperview];
    self.btnBack = NULL;
    [btnDone_ removeFromSuperview];
    self.btnDone = NULL;
    [self removeFromSuperview];
    [super dealloc];
}

-(id) initWithFrame: (CGRect) frameRect
           editArea: (void*) editArea
                   : (std::string)sTitile
                   : (std::string)sPlaceholder
                   : (std::string)sText
                   : (int)nMaxLength
                   : (float)fTitleHeight
                   : (int)nTitleFont
{
    
    if (self = [super initWithFrame:frameRect])
    {
        auto glview = cocos2d::Director::getInstance()->getOpenGLView();
        CCEAGLView *eaglview = static_cast<CCEAGLView *>(glview->getEAGLView());
        float fFactor = eaglview.contentScaleFactor;
        fTitleHeight /= fFactor;
        nTitleFont /= fFactor;
        maxLength_ = nMaxLength;
        self.backgroundColor = [UIColor whiteColor];
        self.viewMenuBar = [[UIView alloc]initWithFrame:CGRectMake(0, 0, frameRect.size.width, fTitleHeight)];
        [viewMenuBar_ setBackgroundColor:COLOR(61, 59, 63, 1.0)];
        [self addSubview:viewMenuBar_];
        self.lblTitle = [[UILabel alloc]init];
        [lblTitle_ setFont:[UIFont systemFontOfSize: nTitleFont]];
        lblTitle_.textAlignment = NSTextAlignment::NSTextAlignmentCenter;
        [lblTitle_ setText:[NSString stringWithUTF8String:sTitile.c_str()]];
        [lblTitle_ setTextColor:[UIColor whiteColor]];
        [self addSubview:lblTitle_];
        
        btnBack_ = [UIButton buttonWithType:UIButtonTypeCustom];
        btnBack_.frame = CGRectMake(0, 0, fTitleHeight * 3 / 2, fTitleHeight);
        [btnBack_ setTitle:@"返回" forState:UIControlStateNormal];
        [btnBack_.titleLabel setFont:[UIFont systemFontOfSize:nTitleFont - 2]];
        [btnBack_ setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
        [btnBack_ setTitleColor:[UIColor grayColor] forState:UIControlStateHighlighted];
        [btnBack_ setTag:TAG_BACK];
        [btnBack_ addTarget:self action:@selector(action:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:btnBack_];
        
        btnDone_ = [UIButton buttonWithType:UIButtonTypeCustom];
        btnDone_.frame = CGRectMake(frameRect.size.width - fTitleHeight * 3 / 2, 0, fTitleHeight * 3 / 2, fTitleHeight);
        [btnDone_ setTitle:@"完成" forState:UIControlStateNormal];
        [btnDone_.titleLabel setFont:[UIFont systemFontOfSize:nTitleFont - 2]];
        [btnDone_ setTitleColor:[UIColor greenColor] forState:UIControlStateNormal];
        [btnDone_ setTitleColor:COLOR(51, 96, 33 , 1.0) forState:UIControlStateHighlighted];
        [btnDone_ setTag:TAG_DONE];
        [btnDone_ addTarget:self action:@selector(action:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:btnDone_];
        
        
        self.tvEdit = [[[UITextView alloc] initWithFrame: CGRectMake(0, fTitleHeight, frameRect.size.width, frameRect.size.height - fTitleHeight)] autorelease];
        [tvEdit_ setTextColor:[UIColor blackColor]];
        tvEdit_.font = [UIFont systemFontOfSize:nTitleFont];
        tvEdit_.backgroundColor = [UIColor clearColor];
        tvEdit_.delegate = self;
        tvEdit_.returnKeyType = UIReturnKeyDefault;
        tvEdit_.text = [NSString stringWithUTF8String:sText.c_str()];
        [self addSubview:self.tvEdit];
        self.editArea = editArea;
    }
    return self;
}

-(void) layoutSubviews
{
    [super layoutSubviews];
    self.lblTitle.frame = viewMenuBar_.frame;
}

- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text
{
    if (maxLength_ < 0)
    {
        return YES;
    }
    
    NSUInteger oldLength = [tvEdit_.text length];
    NSUInteger replacementLength = [text length];
    NSUInteger rangeLength = range.length;
    
    NSUInteger newLength = oldLength - rangeLength + replacementLength;
    
    return newLength <= maxLength_ || newLength <= oldLength;
}

-(void) showDialog
{
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *eaglview = (CCEAGLView *) view->getEAGLView();
    [eaglview addSubview:self];
    [tvEdit_ becomeFirstResponder];
}

-(void)action:(id)sender
{
    if ([sender tag] == TAG_DONE)
    {
        NSString* source = self.tvEdit.text;
        NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:@"[^\\u0020-\\u007E\\u00A0-\\u00BE\\u2E80-\\uA4CF\\uF900-\\uFAFF\\uFE30-\\uFE4F\\uFF00-\\uFFEF\\u0080-\\u009F\\u2000-\\u201F\r\n]"
                                                                               options:NSRegularExpressionCaseInsensitive
                                                                                 error:nil];
        self.tvEdit.text = [regex stringByReplacingMatchesInString:source
                                                           options:0
                                                             range:NSMakeRange(0, [source length])
                                                      withTemplate:@""];
        std::string sText = [self.tvEdit.text UTF8String];
        getEditAreaImplIOS()->getDelegate()->editAreaEditFinish(sText);
    }
    if (editArea_)
    {
        getEditAreaImplIOS()->clearView();
    }
}

-(id)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    UIView *hitView = [super hitTest:point withEvent:event];
    if (hitView == self)
    {
        return nil;
    }
    else
    {
        return hitView;
    }
}

@end

NS_CC_BEGIN

namespace ui {
    
    EditAreaImpl* __createSystemEditArea(EditArea* pEditArea)
    {
        return new EditAreaImplIOS(pEditArea);
    }
    
    EditAreaImplIOS::EditAreaImplIOS(EditArea* pEditArea)
    : EditAreaImpl(pEditArea)
    {
        m_pEditAreaView = nullptr;
    }
    
    EditAreaImplIOS::~EditAreaImplIOS()
    {
        if (m_pEditAreaView)
        {
            [m_pEditAreaView setEditArea:nullptr];
        }
        clearView();
        m_pEditAreaView = nullptr;
        m_pDelegate = nullptr;
        m_pEditArea = nullptr;
    }
    
    void EditAreaImplIOS::showDialog(const std::string& sTitle,
                                     const std::string& sPlaceholder,
                                     const std::string& sText,
                                     const int          nMaxLen,
                                     const float        fTitleHeight,
                                     const int          nTitleFont)
    {
        auto glview = cocos2d::Director::getInstance()->getOpenGLView();
        Size size = glview->getFrameSize();
        CGRect rect = CGRectMake(0, 0, size.width, size.height);
        
        CCEAGLView *eaglview = static_cast<CCEAGLView *>(glview->getEAGLView());
        float factor = eaglview.contentScaleFactor;
        rect.size.width /= factor;
        rect.size.height /= factor;
        
        clearView();
        
        m_pEditAreaView = [[EditAreaView alloc] initWithFrame:rect editArea:this :sTitle :sPlaceholder :sText :nMaxLen: fTitleHeight: nTitleFont];
        [m_pEditAreaView retain];
        [m_pEditAreaView showDialog];
    }
    
    void EditAreaImplIOS::clearView()
    {
        if (m_pEditAreaView)
        {
            [m_pEditAreaView setEditArea:nullptr];
            [m_pEditAreaView removeFromSuperview];
            [m_pEditAreaView release];
            m_pEditAreaView = nullptr;
        }
    }
}
NS_CC_END
#endif