package com.k7k7.sdk.x5;

import java.util.HashMap;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.pm.ActivityInfo;
import android.graphics.Color;
import android.util.Log;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.TextView;

import com.k7k7.sdk.x5.wendu.JsApi;
import com.k7k7.sdk.x5.wendu.JsEchoApi;
import com.k7k7.sdk.x5.wendu.dsbridge.CompletionHandler;
import com.k7k7.sdk.x5.wendu.dsbridge.DWebView;
import com.k7k7.sdk.x5.wendu.dsbridge.OnReturnValue;
import com.k7k7.shengji.AppActivity;
import com.tencent.smtt.sdk.QbSdk;

public class X5WebViewUtil {
	public static FrameLayout thisLayout = null;
	public static String thisSUrl = "";
	public static Cocos2dxActivity thisActivity = null;
	private static DWebView dWebView = null;
	private static View pView = null;
	private static TextView pChild = null;
	private static Boolean isShow = true;
	// 主游戏屏幕方向
	private static int mGameSO = ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE;
	// 页面屏幕方向
	private static int mWebSO = ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT;
	
	private static int nCallId = 0;
	private static HashMap<String, CompletionHandler<String>> callBackQueue = new HashMap();

	public static void init (Cocos2dxActivity activity, FrameLayout layout) {
		thisActivity = activity;
		thisLayout = layout;
		
        //初始化 x5内核 耗时操作放到新的线程
        new Thread(new Runnable() {
            @Override
            public void run() {

        		QbSdk.PreInitCallback cb = new QbSdk.PreInitCallback() {
        			
        			@Override
        			public void onViewInitFinished(boolean arg0) {
        				//x5內核初始化完成的回调，为true表示x5内核加载成功，否则表示x5内核加载失败，会自动切换到系统内核。
        				Log.d("X5WebViewUtil", ">>>>>> onViewInitFinished is " + arg0);
        			}
        			
        			@Override
        			public void onCoreInitFinished() {
        				Log.d("X5WebViewUtil", ">>>>>> onCoreInitFinished");
        			}
        		};
        		//x5内核初始化接口
        		QbSdk.setDownloadWithoutWifi(true);
        		QbSdk.initX5Environment(thisActivity.getApplicationContext(),  cb);
            }
        }).start();
	}
	
	// 打开页面
	public static void openWebView(String sUrl, int nDir) {
		reset();
		if (nDir == 0)
		{
			mWebSO = ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE;
		}
		else 
		{
			mWebSO = ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT;
		}
		Log.d("X5WebViewUtile", "openWebView >>> " + sUrl);
		thisSUrl = sUrl;
		isShow = true;
    	AppActivity.changeOrientation(mWebSO);
		thisActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
	            getInstance().loadUrl(thisSUrl);
			}
		});
	}
	
	// 关闭页面
	public static void closeWebView() {
		Log.d("X5WebViewUtile", "closeWebView");
		isShow = false;
		thisActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if (dWebView != null) {
	            	thisLayout.removeView(dWebView);
	            	dWebView.loadUrl("");
	            	dWebView.destroy();
	            	dWebView = null;
	            	thisLayout.removeView(pView);
	            	pView = null;
	            	thisLayout.removeView(pChild);
	            	pChild = null;
	        		AppActivity.changeOrientation(mGameSO);
	        		onGameFinish();
	            }
			}
		});
	}
	
	// 打开多乐币商城
	public static void openCoinShop() {
		thisActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				AppActivity.changeOrientation(mGameSO);
				hideWebView();
			}
		});
	}
	
	public static void showDialog(final String sCon) {
		thisActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				
				AlertDialog.Builder builder = new AlertDialog.Builder(thisActivity)
				.setMessage(sCon)
				.setPositiveButton("确定", new DialogInterface.OnClickListener() {	
					@Override
					public void onClick(DialogInterface dialog, int arg1) {
						Log.d("AlertDialog", "点确定");
						X5WebViewUtil.callCocos("confirmBuy", "{}", null);
						dialog.dismiss();
					}
				})
				.setNegativeButton("取消", new DialogInterface.OnClickListener() {	
					@Override
					public void onClick(DialogInterface dialog, int arg1) {
						Log.d("AlertDialog", "点取消");
						dialog.dismiss();
					}
				});
//				.setOnKeyListener(new DialogInterface.OnKeyListener() {
//					@Override
//					public boolean onKey(DialogInterface dialog, int arg1, KeyEvent arg2) {
//						Log.d("AlertDialog", "按键");
//						dialog.dismiss();
//						return false;
//					}
//				});
				builder.create().show();
			}
		});
	}

	//--------------- Cocos调JS
	public static void callJs(final String sKey, final String sJson) {
		Log.d("X5WebViewUtile", "callJs >>> key = " + sKey + "  json = " + sJson);
		thisActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Log.d("X5WebViewUtile", "callJs >>> key = " + sKey + "  json = " + sJson);
				dWebView.callHandler("callJs", new Object[]{sKey, sJson}, new OnReturnValue<String>() {
			        @Override
			        public void onValue(String sValue) {
			        	// 回调方法
						Log.d("X5WebViewUtile", "onReturn >>> " + sValue);
			        }
				});
			}
		});
	}
	
	// 只支持一个页面
	private static DWebView getInstance() {
    	if (dWebView == null) {
            FrameLayout.LayoutParams lParams = new FrameLayout.LayoutParams(
                    FrameLayout.LayoutParams.MATCH_PARENT,
                    FrameLayout.LayoutParams.MATCH_PARENT);
    		pView = new View(Cocos2dxActivity.getContext());
    		thisLayout.addView(pView, lParams);
    		pView.setBackgroundColor(Color.parseColor("#333333"));
            pChild = new TextView(pView.getContext());
            pChild.setTextSize(TypedValue.COMPLEX_UNIT_SP, 15);
            pChild.setTextColor(Color.parseColor("#FFFFFF"));
            pChild.setText("精彩总在缓存后……");
            pChild.setGravity(Gravity.CENTER);
            lParams.gravity = Gravity.CENTER;
            thisLayout.addView(pChild, lParams);
    		
        	dWebView = new DWebView(thisActivity.getApplicationContext());
            thisLayout.addView(dWebView, lParams);
            dWebView.addJavascriptObject(new JsApi(), null);
            dWebView.addJavascriptObject(new JsEchoApi(), "echo");
    	}
    	return dWebView;
	}
	
	// 隐藏页面
	private static void hideWebView() {
		if (isShow) {
			isShow = false;
			pView.setVisibility(View.GONE);
			pChild.setVisibility(View.GONE);
			dWebView.setVisibility(View.GONE);
			callJs("webViewHide", "{}");
		}
	}

    private static String getNewCallId() {
    	nCallId = nCallId % 1000000;
    	return "key" + nCallId++;
    }
    
    private static void reset() {
    	isShow = true;
    	nCallId = 0;
    	callBackQueue.clear();
    }
	
	// 显示页面
	public static void showWebView() {
		if (!isShow) {
			isShow = true;
			thisActivity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					Log.d("X5WebViewUtil", "showWebView");
					AppActivity.changeOrientation(mWebSO);
					pView.setVisibility(View.VISIBLE);
					pChild.setVisibility(View.VISIBLE);
					dWebView.setVisibility(View.VISIBLE);
					callJs("webViewShow", "{}");
				}
			});
		}
	}
	
	// JS调Cocos
	public static void callCocos(final String sKey, final String sJson, CompletionHandler<String> handler) {
		final String sCallId = getNewCallId();
		callBackQueue.put(sCallId, handler);
		thisActivity.runOnGLThread(new Runnable() {
			@Override
			public void run() {
				onCallCocos(sKey, sJson, sCallId);
			}
		});
	}
	
	// JS调Cocos的回调
	public static void callCocosComplete(final String sJson, final String sCallId) {
		thisActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Log.d("callCocosComplete", sJson);
				CompletionHandler<String> handler = callBackQueue.remove(sCallId);
				if (handler != null)
				{
					handler.complete(sJson);
				}
			}
		});
	}

	public static native void onCallCocos(String sKey, String sJson, String sCallId);
	public static native void onGameFinish();

}
