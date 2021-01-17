package com.k7k7.shengji;

import android.app.Application;
import android.util.Log;

import com.umeng.message.IUmengRegisterCallback;
import com.umeng.message.PushAgent;

public class MainApplication extends Application {

	@Override
	public void onCreate() {
		super.onCreate();
		
		// 友盟推送
		PushAgent mPushAgent = PushAgent.getInstance(this);
		mPushAgent.setDebugMode(false);
		mPushAgent.register(new IUmengRegisterCallback() {
			
			@Override
			public void onSuccess(String paramString) {
				Log.d("", "PushAgent.register --- onSuccess");
				Log.d("", "DEVICE_TOKEN=[" + paramString + "]");
			}
			
			@Override
			public void onFailure(String paramString1, String paramString2) {
				Log.d("", "PushAgent.register --- onFailure");
				Log.d("", "MESSAGE=[" + paramString1 + " " + paramString2 + "]");
			}
		});
	}
	
}