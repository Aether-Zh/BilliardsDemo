package com.k7k7.sdk.ali;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.widget.Toast;

import com.alipay.sdk.app.PayTask;

public class AliSdkUtil {
	
	private static Cocos2dxActivity thisActivity = null;
	//商户PID
	public static final String PARTNER = "2088311617147762";
	//商户收款账号
	public static final String SELLER = "duole@7k7k.com";
	
	private static final int SDK_PAY_FLAG = 1;

	private static final int SDK_CHECK_FLAG = 2;
	
	public static String RSA_PRIVATE = "";
	
	private static Handler mHandler = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case SDK_PAY_FLAG: {
				PayResult payResult = new PayResult((String) msg.obj);
				String resultStatus = payResult.getResultStatus();
				// 判断resultStatus 为“9000”则代表支付成功，具体状态码代表含义可参考接口文档
				if (TextUtils.equals(resultStatus, "9000")) {
					Toast.makeText(thisActivity, "支付宝支付成功", Toast.LENGTH_SHORT).show();
					thisActivity.runOnGLThread(new Runnable() {
						
						@Override
						public void run() {
							onPayComplete();
						}
					});
				} else {
					// 判断resultStatus 为非“9000”则代表可能支付失败
					// “8000”代表支付结果因为支付渠道原因或者系统原因还在等待支付结果确认，最终交易是否成功以服务端异步通知为准（小概率状态）
					if (TextUtils.equals(resultStatus, "8000")) {
						Toast.makeText(thisActivity, "支付宝支付结果确认中", Toast.LENGTH_SHORT).show();
					} else {
						// 其他值就可以判断为支付失败，包括用户主动取消支付，或者系统返回的错误
						Toast.makeText(thisActivity, "支付宝支付失败", Toast.LENGTH_SHORT).show();
					}
				}
				break;
			}
			case SDK_CHECK_FLAG: {
				Toast.makeText(thisActivity, "检查结果为：" + msg.obj, Toast.LENGTH_SHORT).show();
				break;
			}
			default:
				break;
			}
		};
	};
	
	public static void init(Cocos2dxActivity activity) {
		thisActivity = activity;
	}
	
	public static void destroy() {
        thisActivity = null;
	}
	
	public static void doSdkPay(final String orderInfo, String sign) {
		try {
			// 仅需对sign 做URL编码
			sign = URLEncoder.encode(sign, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}

		// 完整的符合支付宝参数规范的订单信息
		final String payInfo = orderInfo + "&sign=\"" + sign + "\"&"
				+ getSignType();
		Runnable payRunnable = new Runnable() {
			@Override
			public void run() {
				// 构造PayTask 对象
				PayTask alipay = new PayTask(thisActivity);
				// 调用支付接口，获取支付结果
				String result = alipay.pay(payInfo, true);
				Message msg = new Message();
				msg.what = SDK_PAY_FLAG;
				msg.obj = result;
				mHandler.sendMessage(msg);
			}
		};

		// 必须异步调用
		Thread payThread = new Thread(payRunnable);
		payThread.start();
	}
	
	/**
	 * get the sign type we use. 获取签名方式
	 * 
	 */
	public static String getSignType() {
		return "sign_type=\"RSA\"";
	}
	
	public static native void onPayComplete();
}
