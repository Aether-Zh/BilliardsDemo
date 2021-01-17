package com.k7k7.sdk.sobotsdk;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONException;
import org.json.JSONObject;

import android.os.Build;
import android.util.Log;

import com.k7k7.jniutil.PlatformFunction;
import com.k7k7.jniutil.SysUtil;
import com.k7k7.shengjihd.R;

public class SobotSdkUtil {
	private static Cocos2dxActivity thisActivity = null;

	public static void init(Cocos2dxActivity activity) {
		thisActivity = activity;
	}
	
	public static void showFeedback() {
		// 企业编号
		String enterpriseId = "a366c3af16654be0a17b9451be999cb0";
		// 技能组编号
		String skillSetId = "488a12f5f1304097ace3f68105a6ee18";
		// 反馈游戏名称
		String gameName = "升级";
		// 反馈用户昵称
		String nickName = PlatformFunction.getFeedbackUserId();
		if (nickName.isEmpty()) {
			nickName = String.format("单机用户@%s", gameName);
		} else {
			nickName = String.format("联网用户%s@%s", nickName, gameName);
		}
		Log.i("SobotSdkUtil", "nickName = " + nickName);
		// 设备唯一识别码
		String deviceId = SysUtil.getInstance().getMobileID();
		// 反馈用户唯一标识
		String userId = deviceId;
		// 反馈附加信息
		String appInfo = String.format("%s/%s/%d", thisActivity.getString(R.string.app_name), PlatformFunction.getVersionName(), PlatformFunction.getVersionCode());
		String deviceInfo = String.format("%s/%s/%s", Build.BRAND, Build.MODEL, Build.VERSION.RELEASE);
		JSONObject json = new JSONObject();
		try {
			json.put("应用信息", appInfo);
			json.put("渠道信息", PlatformFunction.getChannelType());
			json.put("设备信息", deviceInfo);
			json.put("设备编号", deviceId);
		} catch (JSONException e) {
			e.printStackTrace();
		}
		String url = String.format(
				"https://www.sobot.com/chat/h5/index.html?source=2&sysNum=%s&groupId=%s&partnerId=%s&uname=%s&params=%s",
				enterpriseId, skillSetId, userId, nickName, json.toString());
		PlatformFunction.openURL(url);
	}
}
