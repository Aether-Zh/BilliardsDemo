#import <UIKit/UIKit.h>
#import "WXApi.h"

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate, NSURLConnectionDelegate, WXApiDelegate>

@property(nonatomic, readonly) RootViewController* viewController;

+ (UIViewController *) currentViewController;

- (void)launchUMessageWithOptions:(NSDictionary *)launchOptions;
- (void)launchUMSocialWithOptions:(NSDictionary *)launchOptions;

+(void) screenShotAction:(NSString *)imagePath;
+(void) savingImageIsFinished:(UIImage *)image didFinishSavingWithError:(NSError *)error contextInfo:(void *)contextInfo;
-(void) onResp:(BaseResp*)resp;
@end

