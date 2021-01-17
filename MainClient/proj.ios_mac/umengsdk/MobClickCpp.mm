#include "MobClickCpp.h"
#import "UMMobClick/MobClick.h"
#import "UMMobClick/MobClickGameAnalytics.h"


#define UMENG_SDK_WRAPPER_TYPE                      @"Cocos2d-x"
#define UMENG_SDK_WRAPPER_VERSION                   @"4.7.0"


namespace umeng {
    
    void MobClickCpp::setCheckDevice(bool value){
        //for android
    }
    void MobClickCpp::setSessionIdleLimit(int seconds){
        //for android
    }
    void MobClickCpp::setLogEnabled(bool value){
        BOOL valTmp = value?YES:NO;
        [MobClick setLogEnabled:valTmp];
        
    }
    void MobClickCpp::setEncryptEnabled(bool value){
        BOOL valTmp = value?YES:NO;
        [MobClick setEncryptEnabled:valTmp];
    }
    void MobClickCpp::setAppVersion(const char *appVersion){
        if(!appVersion || strlen(appVersion) == 0){
            NSLog(@"(MobClickCpp::setAppVersion)appversion can't be NULL or \"\"!");
            return;
        }
        [MobClick setAppVersion:[NSString stringWithUTF8String:appVersion]];
    }
    void MobClickCpp::doNotCallItFromYourCode(const char * appkey, const char * channel){
        if (!appkey || strlen(appkey) == 0) {
            NSLog(@"(MobClickCpp::startWithAppkey) appKey can not be NULL or \"\"!");
            return;
        }
        [MobClick setWrapperType:UMENG_SDK_WRAPPER_TYPE wrapperVersion:UMENG_SDK_WRAPPER_VERSION];
        UMConfigInstance.appKey = [NSString stringWithUTF8String:appkey];
        UMConfigInstance.channelId = [NSString stringWithUTF8String:channel];
        UMConfigInstance.eSType = E_UM_GAME; // 仅适用于游戏场景，应用统计不用设置
        
        [MobClick startWithConfigure:UMConfigInstance];
    }
    void MobClickCpp::event(const char * eventId, const char * label){
        if(label){
            [MobClick event:[NSString stringWithUTF8String:eventId] label:[NSString stringWithUTF8String:label]];
        }else{
            [MobClick event:[NSString stringWithUTF8String:eventId]];
        }
    }
    void MobClickCpp::event(const char * eventId, eventDict * attributes, int counter){
        NSMutableDictionary *dict = [[NSMutableDictionary alloc] init];
        eventDict::iterator it;
        for(it=attributes->begin();it!=attributes->end();++it)
        {
            NSString* key = [NSString stringWithUTF8String:it->first.c_str()];
            NSString* obj = [NSString stringWithUTF8String:it->second.c_str()];
            [dict setObject:obj forKey:key];
        }
        NSString* eId = [NSString stringWithUTF8String:eventId];
        [MobClick event:eId attributes:dict counter:counter];
        [dict release];
    }
        
    void MobClickCpp::event(const char * eventId, eventDict * attributes){
        NSMutableDictionary *dict = [[NSMutableDictionary alloc] init];
        eventDict::iterator it;
        for(it=attributes->begin();it!=attributes->end();++it)
        {
            NSString* key = [NSString stringWithUTF8String:it->first.c_str()];
            NSString* obj = [NSString stringWithUTF8String:it->second.c_str()];
            [dict setObject:obj forKey:key];
        }
        NSString* eId = [NSString stringWithUTF8String:eventId];
        [MobClick event:eId attributes:dict];
        [dict release];
    }
    void MobClickCpp::beginLogPageView(const char *pageName){
        NSString* name = [NSString stringWithUTF8String:pageName];
        [MobClick beginLogPageView:name];
    }
    void MobClickCpp::endLogPageView(const char *pageName){
        NSString* name = [NSString stringWithUTF8String:pageName];
        [MobClick endLogPageView:name];
    }
    void MobClickCpp::profileSignIn(const char * puid, const char * provider){
        NSString* uid = [NSString stringWithUTF8String:puid];
        if(provider){
            NSString* pro = [NSString stringWithUTF8String:provider];
            [MobClick profileSignInWithPUID:uid provider:pro];
        }else{
            [MobClick profileSignInWithPUID:uid];
        }
    }
    
    void MobClickCpp::profileSignOff(){
        [MobClick profileSignOff];
    }
    void MobClickCpp::setUserLevel(int level){
        [MobClickGameAnalytics setUserLevelId:level];
    }
    void MobClickCpp::startLevel(const char * level){
        [MobClickGameAnalytics startLevel:[NSString stringWithUTF8String:level]];
    }
    void MobClickCpp::finishLevel(const char * level){
        [MobClickGameAnalytics finishLevel:[NSString stringWithUTF8String:level]];
    }
    
    void MobClickCpp::failLevel(const char * level){
        [MobClickGameAnalytics failLevel:[NSString stringWithUTF8String:level]];
    }
    
    void MobClickCpp::pay(double cash, int src, double co){
        [MobClickGameAnalytics pay:cash source:src coin:co];
    }
    
    void MobClickCpp::pay(double cash, int src, const char * item, int am, double pr){
        NSString* it = [NSString stringWithUTF8String:item];
        [MobClickGameAnalytics pay:cash source:src item:it amount:am price:pr];
    }
    
    void MobClickCpp::buy(const char *item, int am, double pr){
        NSString* it = [NSString stringWithUTF8String:item];
        [MobClickGameAnalytics buy:it amount:am price:pr];
    }
    
    void MobClickCpp::use(const char * item, int am, double pr){
        NSString* it = [NSString stringWithUTF8String:item];
        [MobClickGameAnalytics use:it amount:am price:pr];
    }
    void MobClickCpp::bonus(double coin, int src){
        [MobClickGameAnalytics bonus:coin source:src];
    }
    void MobClickCpp::bonus(const char * item, int am, double pr, int src){
        NSString* it = [NSString stringWithUTF8String:item];
        [MobClickGameAnalytics bonus:it amount:am price:pr source:src];
    }
    void MobClickCpp::setLatency(unsigned int latency){
    }
}
