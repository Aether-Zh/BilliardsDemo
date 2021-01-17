//
//  PlatformFunction.cpp
//  GouJi
//
//  Created by mtt on 14-9-29.
//
//

#include "PlatformFunction.h"
#include "Core/AppVersion.h"
#include "Models/AppModel.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "AppDelegate.h"
#include "Util/Utils.h"
#include "Core/AppEvent.h"
#include "Core/AppDef.h"

USING_NS_CC;
using namespace std;
using namespace util;

#define CLASS_NAME "com/k7k7/jniutil/PlatformFunction"
#define UPDATE_CLASS_NAME "com/k7k7/jniutil/CheckUpdate"
#define FEEDBACK_CLASS_NAME "com/k7k7/sdk/sobotsdk/SobotSdkUtil"
#define SYS_UTIL "com/k7k7/jniutil/SysUtil"
#define APP_ACTIVITY "com/k7k7/shengji/AppActivity"

static string s_sEventOnFinish = "";
static string s_sEventSaveImageFinish = "";
static string s_sSchemePath = "";
static string s_sSchemeData = "";
static string s_sEventAuthFinish = "";

void PlatformFunction::openURL(const string &sUrl)
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "openURL",
                                       "(Ljava/lang/String;)V"))
    {
        jstring url = obMethodInfo.env->NewStringUTF(sUrl.c_str());

        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               url);

        obMethodInfo.env->DeleteLocalRef(url);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

void PlatformFunction::openApplication(const string &sLinkUrl, const string &sDownloadUrl)
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "openApplication",
                                       "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring linkUrl = obMethodInfo.env->NewStringUTF(sLinkUrl.c_str());
        jstring downloadUrl = obMethodInfo.env->NewStringUTF(sDownloadUrl.c_str());
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               linkUrl,
                                               downloadUrl);
        obMethodInfo.env->DeleteLocalRef(linkUrl);
        obMethodInfo.env->DeleteLocalRef(downloadUrl);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

bool PlatformFunction::canOpenApplication(const std::string &sLinkUrl)
{
	JniMethodInfo obMethodInfo;
	jboolean bRet = false;
	if (JniHelper::getStaticMethodInfo(obMethodInfo,
	                                   CLASS_NAME,
	                                   "canOpenApplication",
	                                   "(Ljava/lang/String;)Z"))
	{
		jstring url = obMethodInfo.env->NewStringUTF(sLinkUrl.c_str());

		bRet = obMethodInfo.env->CallStaticBooleanMethod(obMethodInfo.classID,
		                                               	 obMethodInfo.methodID,
		                                                 url);

		obMethodInfo.env->DeleteLocalRef(url);
		obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
	}
	return bRet == JNI_TRUE ? true : false;
}

float PlatformFunction::getBatteryLevel()
{
    JniMethodInfo obMethodInfo;
    jfloat fRet = 0;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "getBatteryLevel",
                                       "()F"))
    {
        fRet = obMethodInfo.env->CallStaticFloatMethod(obMethodInfo.classID,
                                                       obMethodInfo.methodID);

        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
    return fRet;
}

void PlatformFunction::showFeedback()
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "showFeedback",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);

        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

void PlatformFunction::sharePage(SocialPlatform ePlatform,
                                 SocialPageCategory eCategory,
                                 const string &sText,
                                 const string &sTitle,
                                 const string &sUrl,
                                 const string &sEventOnFinish)
{
    s_sEventOnFinish = sEventOnFinish;

    if (ePlatform == PlatformFunction::SocialPlatform::WECHATSESSION)
    {
       Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVT_SHARE_START);
    }
    else
    {
       // 重置分享开始时间，只有微信好友分享有时间控制成功或失败
        AppModel::getInstance()->m_pFreeModel->updateShareStartTime(GAME_DATA_UNKNOWN);
    }

    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "sharePage",
                                       "(IILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
    {
    	jint platform = (jint)ePlatform;
    	jint category = (jint)eCategory;
    	jstring text = obMethodInfo.env->NewStringUTF(sText.c_str());
    	jstring title = obMethodInfo.env->NewStringUTF(sTitle.c_str());
        jstring url = obMethodInfo.env->NewStringUTF(sUrl.c_str());

        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               platform,
                                               category,
                                               text,
                                               title,
                                               url);

        obMethodInfo.env->DeleteLocalRef(text);
        obMethodInfo.env->DeleteLocalRef(title);
        obMethodInfo.env->DeleteLocalRef(url);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

void PlatformFunction::shareImage(SocialPlatform ePlatform,
                                  const string &sAbsoluteImagePath,
                                  const string &sEventOnFinish)
{
    s_sEventOnFinish = sEventOnFinish;

    if (ePlatform == PlatformFunction::SocialPlatform::WECHATSESSION)
    {
       Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVT_SHARE_START);
    }
    else 
    {
       // 重置分享开始时间，只有微信好友分享有时间控制成功或失败
        AppModel::getInstance()->m_pFreeModel->updateShareStartTime(GAME_DATA_UNKNOWN);
    }

    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "shareImage",
                                       "(ILjava/lang/String;)V"))
    {
    	jint platform = (jint)ePlatform;
    	jstring absoluteImagePath = obMethodInfo.env->NewStringUTF(sAbsoluteImagePath.c_str());

        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               platform,
                                               absoluteImagePath);

        obMethodInfo.env->DeleteLocalRef(absoluteImagePath);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

void PlatformFunction::shareToWXMiniGame(const std::string& sUrl, const std::string& sUserName, const std::string& sThumbUrl, const std::string &sEventOnFinish)
{
    s_sEventOnFinish = sEventOnFinish;

    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVT_SHARE_START);

    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "shareToWXMiniGame",
                                       "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
    {
      
        jstring url = obMethodInfo.env->NewStringUTF(sUrl.c_str());
        jstring userName = obMethodInfo.env->NewStringUTF(sUserName.c_str());
        jstring thumbUrl = obMethodInfo.env->NewStringUTF(sThumbUrl.c_str());
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               url,
                                               userName,
                                               thumbUrl);

        obMethodInfo.env->DeleteLocalRef(url);
        obMethodInfo.env->DeleteLocalRef(userName);
        obMethodInfo.env->DeleteLocalRef(thumbUrl);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

void PlatformFunction::onShareFinish(bool bSuccess)
{
    if (s_sEventOnFinish.empty())
    {
        return;
    }
    Value obSuccess(bSuccess);
    Value obEvent(s_sEventOnFinish);
    ValueMap oMap;
    oMap.insert(make_pair("eventname", obEvent));
    oMap.insert(make_pair("success", obSuccess));
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVT_SHARE_FINISH, &oMap);
    s_sEventOnFinish.clear();
}

void PlatformFunction::removeAllLocalNotification()
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "removeAllLocalNotification",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

void PlatformFunction::addLocalNotification(const string &sText,
                             const string &sTitle,
                             const long &nDelay,
                             const string &sKey)
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "addLocalNotification",
                                       "(Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;)V"))
    {
    	jstring text = obMethodInfo.env->NewStringUTF(sText.c_str());
    	jstring title = obMethodInfo.env->NewStringUTF(sTitle.c_str());
    	jlong delay = (jlong)nDelay;
    	jstring key = obMethodInfo.env->NewStringUTF(sKey.c_str());

        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               text,
                                               title,
                                               delay,
                                               key);

        obMethodInfo.env->DeleteLocalRef(text);
        obMethodInfo.env->DeleteLocalRef(title);
        obMethodInfo.env->DeleteLocalRef(key);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

void PlatformFunction::copyToDipboard(const string &sText)
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "copyToDipboard",
                                       "(Ljava/lang/String;)V"))
    {
    	jstring text = obMethodInfo.env->NewStringUTF(sText.c_str());

        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               text);

        obMethodInfo.env->DeleteLocalRef(text);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

string PlatformFunction::getCopyText()
{
    string sRet = "";
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "getCopyText",
                                       "()Ljava/lang/String;"))
    {
        jstring jstr = static_cast<jstring>(obMethodInfo.env->CallStaticObjectMethod(obMethodInfo.classID, obMethodInfo.methodID));
        
        if(jstr)
        {
            const char* pszChars = obMethodInfo.env->GetStringUTFChars(jstr, nullptr);
            sRet.assign(pszChars);
            obMethodInfo.env->ReleaseStringUTFChars(jstr, pszChars);
        }
        
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
    return sRet;
}

void PlatformFunction::openWeChat()
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "openWeChat",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

void PlatformFunction::shake()
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "shake",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

bool PlatformFunction::isEmulator()
{
#ifdef DEBUG_MODE
	return false;
#endif
	JniMethodInfo obMethodInfo;
	jboolean bRet = false;
	if (JniHelper::getStaticMethodInfo(obMethodInfo,
	                                   CLASS_NAME,
	                                   "isEmulator",
	                                   "()Z"))
	{
		bRet = obMethodInfo.env->CallStaticBooleanMethod(obMethodInfo.classID,
	                                           	   	   	 obMethodInfo.methodID);

	    obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
	}
	return bRet == JNI_TRUE ? true : false;
}

void PlatformFunction::onShowComment(const std::string &sAddress)
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "onShowComment",
                                       "(Ljava/lang/String;)V"))
    {
        jstring address = obMethodInfo.env->NewStringUTF(sAddress.c_str());
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               address);
        obMethodInfo.env->DeleteLocalRef(address);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

void PlatformFunction::onSaveImage(const std::string &sPath, const std::string& sEventOnFinish)
{
	s_sEventSaveImageFinish = sEventOnFinish;
	JniMethodInfo obMethodInfo;
	if (JniHelper::getStaticMethodInfo(obMethodInfo,
									   CLASS_NAME,
	                                   "onSaveImage",
	                                   "(Ljava/lang/String;)V"))
	{
	    jstring path = obMethodInfo.env->NewStringUTF(sPath.c_str());

	    obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
	                                               obMethodInfo.methodID,
	                                               path);

	    obMethodInfo.env->DeleteLocalRef(path);
	    obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
	 }
}

void PlatformFunction::onSaveImageFinish(bool bSuccess)
{
    if (s_sEventSaveImageFinish.empty())
    {
        return;
    }
    EventDispatcher *pDispatcher = Director::getInstance()->getEventDispatcher();
    Value obSuccess(bSuccess);
    pDispatcher->dispatchCustomEvent(s_sEventSaveImageFinish, &obSuccess);
    s_sEventSaveImageFinish.clear();
}

void PlatformFunction::OpenSystemSettings()
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "OpenSystemSettings",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

int PlatformFunction::getConnectedType()
{
  JniMethodInfo obMethodInfo;
  jint nRet = 0;
  if (JniHelper::getStaticMethodInfo(obMethodInfo,
                     CLASS_NAME,
                     "getConnectedType",
                     "()I"))
  {
    nRet = obMethodInfo.env->CallStaticIntMethod(obMethodInfo.classID,
                               obMethodInfo.methodID);

    obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
  }
  return nRet;
}

void PlatformFunction::openAppInAppMarket(const string &sAppLink, const string &sMarketAddress)
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "openAppInAppMarket",
                                       "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring appLink = obMethodInfo.env->NewStringUTF(sAppLink.c_str());
        jstring marketAddress = obMethodInfo.env->NewStringUTF(sMarketAddress.c_str());
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               appLink,
                                               marketAddress);
        obMethodInfo.env->DeleteLocalRef(appLink);
        obMethodInfo.env->DeleteLocalRef(marketAddress);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

void PlatformFunction::openCurrentAppInAppMarket(const string &sMarketAddress)
{
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "openCurrentAppInAppMarket",
                                       "(Ljava/lang/String;)V"))
    {
        jstring marketAddress = obMethodInfo.env->NewStringUTF(sMarketAddress.c_str());
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID,
                                               marketAddress);
        obMethodInfo.env->DeleteLocalRef(marketAddress);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

bool PlatformFunction::canOpenAppMarket(const string &sAddress)
{
    JniMethodInfo obMethodInfo;
    jboolean bRet = false;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "canOpenAppMarket",
                                       "(Ljava/lang/String;)Z"))
    {
      jstring address = obMethodInfo.env->NewStringUTF(sAddress.c_str());
      bRet = obMethodInfo.env->CallStaticBooleanMethod(obMethodInfo.classID,
                                                       obMethodInfo.methodID,
                                                       address);
      obMethodInfo.env->DeleteLocalRef(address);
      obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
    return bRet == JNI_TRUE ? true : false;
}

void PlatformFunction::onSaveSchemesData(const std::string &sPath, const std::string& sData)
{
  s_sSchemePath = sPath;
  s_sSchemeData = sData;
}

string PlatformFunction::getSchemePath()
{
    return s_sSchemePath;
}

string PlatformFunction::getSchemeData()
{
    return s_sSchemeData;
}

// 获取启动耗时(毫秒)
int PlatformFunction::getTimeMillisAppStarted()
{
    JniMethodInfo obMethodInfo;
    jint bRet = 0;
    if (JniHelper::getStaticMethodInfo(obMethodInfo, SYS_UTIL, "getTimeMillisAppStarted", "()I")){
        bRet = obMethodInfo.env->CallStaticIntMethod(obMethodInfo.classID, obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
    return bRet;
}
// 启动状态 true 完成
bool PlatformFunction::getAppInitState()
{
    jboolean bRet = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       APP_ACTIVITY,
                                       "getAppInitState",
                                       "()Z"))
    {
        bRet = obMethodInfo.env->CallStaticBooleanMethod(obMethodInfo.classID,
                                                         obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
#endif
    return bRet == JNI_TRUE ? true : false;
}

// 移除闪屏页
void PlatformFunction::clearWelcomeImage()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo, APP_ACTIVITY, "removeLaunchImage", "()V")){
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID, obMethodInfo.methodID);
        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
#endif
}

void PlatformFunction::doWeChatAuth(const std::string& sAuthEvent)
{
    s_sEventAuthFinish = sAuthEvent;
    JniMethodInfo obMethodInfo;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "doWeChatAuth",
                                       "()V"))
    {
        obMethodInfo.env->CallStaticVoidMethod(obMethodInfo.classID,
                                               obMethodInfo.methodID);

        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
}

bool PlatformFunction::isInstallWeChat()
{
    JniMethodInfo obMethodInfo;
    jboolean bRet = false;
    if (JniHelper::getStaticMethodInfo(obMethodInfo,
                                       CLASS_NAME,
                                       "isInstallWeChat",
                                       "()Z"))
    {
        bRet = obMethodInfo.env->CallStaticBooleanMethod(obMethodInfo.classID,
                                                         obMethodInfo.methodID);

        obMethodInfo.env->DeleteLocalRef(obMethodInfo.classID);
    }
    return bRet == JNI_TRUE ? true : false;
}

extern "C"
{
	JNIEXPORT void JNICALL Java_com_k7k7_jniutil_PlatformFunction_onShareFinish(JNIEnv* env, jobject thiz, jboolean success)
	{
		PlatformFunction::onShareFinish(success);
	}

	JNIEXPORT jstring JNICALL Java_com_k7k7_jniutil_PlatformFunction_getFeedbackUserId(JNIEnv* env, jobject thiz)
	{
		if (AppModel::getInstance()->m_pRemoteModel->isOnline())
		{
			return env->NewStringUTF(util::str_format("%llu", AppModel::getInstance()->m_pUserModel->getMyUserId()));
		}
		return env->NewStringUTF("");
	}

  JNIEXPORT jstring JNICALL Java_com_k7k7_jniutil_PlatformFunction_getSobotAppKey(JNIEnv* env, jobject thiz)
  {
    return env->NewStringUTF(SOBOT_APP_KEY);
  }

  JNIEXPORT jstring JNICALL Java_com_k7k7_jniutil_PlatformFunction_getSobotSkillSetId(JNIEnv* env, jobject thiz)
  {
    return env->NewStringUTF(SOBOT_SKILLSET_ID);
  }

	JNIEXPORT jstring JNICALL Java_com_k7k7_jniutil_PlatformFunction_getChannelType(JNIEnv* env, jobject thiz)
	{
		return env->NewStringUTF(CHANNEL_TYPE);
	}

	JNIEXPORT jstring JNICALL Java_com_k7k7_jniutil_PlatformFunction_getUmengAppKey(JNIEnv* env, jobject thiz)
	{
		return env->NewStringUTF(UMENG_APP_KEY);
	}

	JNIEXPORT jstring JNICALL Java_com_k7k7_jniutil_PlatformFunction_getUmengChannelId(JNIEnv* env, jobject thiz)
	{
		return env->NewStringUTF(UMENG_CHANNEL_ID);
	}

	JNIEXPORT jstring JNICALL Java_com_k7k7_jniutil_PlatformFunction_getWXAppId(JNIEnv* env, jobject thiz)
	{
		return env->NewStringUTF(WX_APP_ID);
	}

	JNIEXPORT jstring JNICALL Java_com_k7k7_jniutil_PlatformFunction_getWXAppSecret(JNIEnv* env, jobject thiz)
	{
		return env->NewStringUTF(WX_APP_SECRET);
	}

	JNIEXPORT jstring JNICALL Java_com_k7k7_jniutil_PlatformFunction_getQQZoneAppId(JNIEnv* env, jobject thiz)
	{
		return env->NewStringUTF(QQZONE_APP_ID);
	}

	JNIEXPORT jstring JNICALL Java_com_k7k7_jniutil_PlatformFunction_getQQZoneAppKey(JNIEnv* env, jobject thiz)
	{
		return env->NewStringUTF(QQZONE_APP_KEY);
	}

	JNIEXPORT jstring JNICALL Java_com_k7k7_jniutil_PlatformFunction_getSinaAppKey(JNIEnv* env, jobject thiz)
	{
		return env->NewStringUTF(SINA_APP_KEY);
	}

	JNIEXPORT jstring JNICALL Java_com_k7k7_jniutil_PlatformFunction_getSinaAppSecret(JNIEnv* env, jobject thiz)
	{
		return env->NewStringUTF(SINA_APP_SECRET);
	}

	JNIEXPORT void JNICALL Java_com_k7k7_jniutil_PlatformFunction_onSaveImageFinish(JNIEnv* env, jobject thiz, jboolean success)
	{
		PlatformFunction::onSaveImageFinish(success);
	}

	JNIEXPORT jstring JNICALL Java_com_k7k7_jniutil_CheckUpdate_getClientVersion(JNIEnv* env, jobject thiz)
	{
		return env->NewStringUTF(CLIENT_VERSION);
	}

  JNIEXPORT void JNICALL Java_com_k7k7_jniutil_PlatformFunction_onSaveSchemesData(JNIEnv* env, jobject thiz, jstring path, jstring data)
  {
    jboolean jresult = true;
    const char* pPath = env->GetStringUTFChars(path, &jresult);
    const char* pData = env->GetStringUTFChars(data, &jresult);
    PlatformFunction::onSaveSchemesData(pPath, pData);
  }

  JNIEXPORT void JNICALL Java_com_k7k7_jniutil_PlatformFunction_onAuthFinish(JNIEnv* env, jobject thiz, jstring accessToken, jstring refreshToken, jstring openId, jboolean error)
  {
    KKWECHATAUTH oData = {};
    oData.sAccessToken = JniHelper::jstring2string(accessToken);
    oData.sRefreshToken = JniHelper::jstring2string(refreshToken);
    oData.sOpenId = JniHelper::jstring2string(openId);
    oData.bError = error;
    EventDispatcher *pDispatcher = Director::getInstance()->getEventDispatcher();
    pDispatcher->dispatchCustomEvent(s_sEventAuthFinish, &oData);
    s_sEventAuthFinish.clear();
  }

  JNIEXPORT jstring JNICALL Java_com_k7k7_jniutil_PlatformFunction_getWXMiniUserName(JNIEnv* env, jobject thiz)
  {
    return env->NewStringUTF(WX_MINIGAME_USERNAME);
  }

  JNIEXPORT jstring JNICALL Java_com_k7k7_jniutil_PlatformFunction_getWXMiniThumbUrl(JNIEnv* env, jobject thiz)
  {
    return env->NewStringUTF(WX_MINIGAME_THUMB_URL);
  }
}
