package com.k7k7.sdk.wechat;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.widget.Toast;

import com.k7k7.jniutil.PlatformFunction;
import com.tencent.mm.sdk.constants.ConstantsAPI;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.modelpay.PayReq;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

public class WeChatSdkUtil {
	private static Cocos2dxActivity thisActivity = null;
	private static IWXAPI api;
	
	public static void init(Cocos2dxActivity activity) {
		thisActivity = activity;
		
		api = WXAPIFactory.createWXAPI(thisActivity, null);
		api.registerApp(PlatformFunction.getWXAppId());
	}
	
	public static void destroy() {
        thisActivity = null;
	}
	
	public static void doSdkPay(
			final String partnerId,
			final String nonceStr,
			final String pack,
			final String prepayId,
			final String timestamp,
			final String sign) {
		Runnable payRunnable = new Runnable() {
			@Override
			public void run() {
				PayReq req = new PayReq();
				req.appId = PlatformFunction.getWXAppId();
				req.partnerId = partnerId;
				req.prepayId = prepayId;
				req.nonceStr = nonceStr;
				req.timeStamp = timestamp;
				req.packageValue = pack;
				req.sign = sign;
				api.sendReq(req);
			}
		};
		// 必须异步调用
		Thread payThread = new Thread(payRunnable);
		payThread.start();
	}
	
	public static void onReq(BaseReq req) {
		
	}
	
	public static void onResp(BaseResp resp) {
		if (resp.getType() == ConstantsAPI.COMMAND_PAY_BY_WX) {
			if (resp.errCode == 0)
			{
				Toast.makeText(thisActivity, "微信支付成功", Toast.LENGTH_LONG).show();
				thisActivity.runOnGLThread(new Runnable() {
					
					@Override
					public void run() {
						onPayComplete();
					}
				});
			}
		}
	}
	
	public static void onShareReq(BaseReq req) {
		
	}
	
	public static void onShareResp(BaseResp resp) {
		System.out.println("WeChatSdkUtil onShareResp errcode" + String.valueOf(resp.errCode));
		if (resp.errCode == 0) {
			thisActivity.runOnGLThread(new Runnable() {
				
				@Override
				public void run() {
					PlatformFunction.onShareFinish(true);
				}
			});
		} else {
			thisActivity.runOnGLThread(new Runnable() {
				
				@Override
				public void run() {
					PlatformFunction.onShareFinish(false);
				}
			});
		}
	}
	
	public static native void onPayComplete();
}
