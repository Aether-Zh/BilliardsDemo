/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.k7k7.shengji;

import java.util.ArrayList;
import java.util.HashMap;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Intent;
import android.net.Uri;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.support.v4.app.ActivityCompat;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageView;

import com.k7k7.jniutil.PlatformFunction;
import com.k7k7.jniutil.SysUtil;
import com.k7k7.sdk.ali.AliSdkUtil;
import com.k7k7.sdk.sobotsdk.SobotSdkUtil;
import com.k7k7.sdk.wechat.WeChatSdkUtil;
import com.k7k7.sdk.x5.X5WebViewUtil;
import com.k7k7.shengjihd.R;
import com.k7k7.update.AppUpdateUtil;
import com.umeng.analytics.mobclick.game.MobClickCppHelper;
import com.umeng.message.PushAgent;

public class AppActivity extends Cocos2dxActivity implements ActivityCompat.OnRequestPermissionsResultCallback  {
	private static Cocos2dxActivity mThis = null;
	// 遮罩层控制相关变量；  
    private static Handler mUIHandler = null;
    private static ImageView mWelcome = null;
    private static ImageView mWelcome2 = null;
	// 授权完成标志
    private static boolean AppInitIsOK = false;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		SysUtil.setStartTime(System.currentTimeMillis());
        SysUtil.reportPageEvent("启动游戏");
		super.onCreate(savedInstanceState);
		// 启动异步加载
        onAsyncLoad();
		Log.i("", "protected void onCreate(Bundle savedInstanceState)");
		// 屏幕常亮
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		addLaunchView();
		// 动态权限申请
        PermissionsUtil.permissionsUtilInit(this);
        // 检测是否需要动态获取权限；
        if(PermissionsUtil.checkPermissions()){
        	SysUtil.setFirstInstall(true);
            SysUtil.reportPageEvent("动态申请申请权限");
            PermissionsUtil.requestRequiredPermissions();
        }
	}
	
	private void launchApp() {
        mThis = this;
        // 屏幕方向
     	changeOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
        X5WebViewUtil.init(this, mFrameLayout);
		// 友盟统计
		MobClickCppHelper.init(this, PlatformFunction.getUmengAppKey(), PlatformFunction.getUmengChannelId());
		// 友盟推送
		PushAgent.getInstance(this).onAppStart();

		Log.i("", "protected void onCreate(Bundle savedInstanceState)");
		SysUtil.getInstance().init(this);
		PlatformFunction.init(this);
		AliSdkUtil.init(this);
		WeChatSdkUtil.init(this);
		SobotSdkUtil.init(this);
		AppUpdateUtil.init(this);

		Uri data = getIntent().getData();
		if (data != null)
		{
			String sPath = data.getPath();
			String sData = data.getQuery();
			PlatformFunction.onSaveSchemesResult(sPath, sData);
		}

		Log.i("", "protected void onCreate(Bundle savedInstanceState)");
	    SysUtil.reportPageEvent("开始进入闪屏页");
		//**** 初始化完毕，cocos层可以继续了 ****//
        AppInitIsOK = true;
    }
	
	// 异步加载
    private void onAsyncLoad() {
        new Thread(new Runnable() {
            @Override
            public void run() {
            	Looper.prepare();
                launchApp();
            }
        }, "AppActivity").start();
    }
	// log闪屏
	protected void addLaunchView() {
        mUIHandler = new Handler();
        // 显示launch image 遮住「黑屏」
        addContentView(createLaunchImage(),
        new WindowManager.LayoutParams(
        WindowManager.LayoutParams.FLAG_FULLSCREEN,
        WindowManager.LayoutParams.FLAG_FULLSCREEN));

        addContentView(createLaunchImage2(),
        new WindowManager.LayoutParams(
        WindowManager.LayoutParams.FLAG_FULLSCREEN,
        WindowManager.LayoutParams.FLAG_FULLSCREEN));
    }

	// 创建一个ImageView，welcome是闪屏图片
    protected ImageView createLaunchImage() {
        mWelcome = new ImageView(this);
        mWelcome.setImageResource(R.drawable.logo_bg);
        mWelcome.setScaleType(ImageView.ScaleType.FIT_XY);
        return mWelcome;
    }
    protected ImageView createLaunchImage2() {
        mWelcome2 = new ImageView(this);
        mWelcome2.setImageResource(R.drawable.logo_icon);
        mWelcome2.setScaleType(ImageView.ScaleType.CENTER_INSIDE);
        return mWelcome2;
    }
	
	public static void removeLaunchImage() {
		mThis.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if (mWelcome != null) {
                    mWelcome.setVisibility(View.GONE);
                }
                if (mWelcome2 != null) {
                    mWelcome2.setVisibility(View.GONE);
                }
                mWelcome = null;
                mWelcome2 = null;
			}
		});
    }
	@Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        PermissionsUtil.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }
	
	@Override
	protected void onResume() {
		super.onResume();
		
		MobClickCppHelper.onResume(this);
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		Log.d("", "onKeyDown");
		return false;
	}
	
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		Log.d("", "onKeyUp");
		return false;
	}

	@Override
	protected void onPause() {
		super.onPause();
		MobClickCppHelper.onPause(this);
    }
	
	@Override
	protected void onDestroy() {
		SysUtil.reportPageEvent("退出游戏");
		super.onDestroy();
		SysUtil.getInstance().destroy();
		PlatformFunction.destroy();
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		PlatformFunction.onActivityResult(requestCode, resultCode, data);
	}
	
		@Override 
	protected void onNewIntent(Intent intent) {
		Uri data = intent.getData();
		if (data != null)
		{
			String sPath = data.getPath();
			String sData = data.getQuery();
			PlatformFunction.onSaveSchemesResult(sPath, sData);
		}
	}
	
	public static void changeOrientation(int type) {
		if (mThis.getRequestedOrientation() != type) {
			mThis.setRequestedOrientation(type);	
		}
	}

	/*
    * 获取app启动状态
    * */
    public static boolean getAppInitState() {
        boolean nRet = false;
        //  用户已经全部授权并且app初始化已经完成，Cocos2dx可以继续往下执行；
        if ((false == PermissionsUtil.checkPermissions()) && AppInitIsOK) {
            nRet = true;
        }
        return nRet;
    }
}
